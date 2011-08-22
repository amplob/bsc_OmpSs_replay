

import sys

def panic (string):
   print 'PANIC: ' + string
   #print >> sys.stderr, 'exited with panic'
   sys.exit(1)
  
STARTING_MPI_PROCESS = -1  

class SMPSs_user_events:
   EVENT_TYPE_TASKID          =7001   
   EVENT_TYPE_TASKNAME_CODED  =7003   
   EVENT_TYPE_CSS_START       =7004
   EVENT_TYPE_PHASEID         =7005
   EVENT_TYPE_CSS_BARRIER     =7006
   EVENT_TYPE_DEPENDENCY      =7007   
   EVENT_TYPE_TASK_PRIORITY   =6999
   
class DependencyCommunicationTags:
   DEPENDENCY_TAG_SPAWNING    =8000
   DEPENDENCY_TAG_INTERTASK   =8001
   DEPENDENCY_TAG_JOINING     =8002
   
   

class TraceRecord(object):
   @staticmethod
   def create_input_record(line):
      tag = line.split('{')[0].strip()
      parameters = line.split('{')[-1].split('}')[0]
      parameters = parameters.split(',')
      if (tag == '"user event"'):
         return UserEvent(tag, *parameters)
      elif (tag == '"CPU burst"'):
         return CPUBurst(tag, *parameters)
      elif (tag == '"block begin"'):
         return BlockBegin(tag, *parameters)
      elif (tag == '"block end"'):
         return BlockEnd(tag, *parameters)
      elif (tag == '"NX send"'):
         return NXSend(tag, *parameters)
      elif (tag == '"NX recv"'):
         return NXRecv(tag, *parameters)
      elif (tag == '"global OP"'):
         return GlobalOp(tag, *parameters)                  
      else:
         return None
   @staticmethod         
   def create_in_dependency (MPI_process, src_threadid, dest_threadid):
      return InDependency(MPI_process, src_threadid, dest_threadid)
   @staticmethod      
   def create_out_dependency (MPI_process, src_threadid, dest_threadid):
      return OutDependency(MPI_process, src_threadid, dest_threadid)      
   @staticmethod      
   def create_user_event (MPI_process, threadid, event_type, event_value):
      return UserEvent ('"user event"', MPI_process, threadid, event_type, event_value)
   @staticmethod      
   def create_empty_CPU_burst (MPI_process, threadid):
      return CPUBurst ('"CPU burst"', MPI_process, threadid, 0.0)      
      
      
   def __str__ (self):
      return self._tag + ' { ' + self._pars_string() + ' };;\n'
   def get_MPI_process(self):
      return self.MPI_process
   def get_taskid(self):
      return self.threadid
   def set_threadid(self, threadid):
      self.threadid = threadid
      
   #change threadid to represent relative task
   def relativize_inside_cut(self, first_task_in_cut, is_currently_in_task, current_absolute_taskid):
      if (is_currently_in_task):
         relative_taskid = current_absolute_taskid - first_task_in_cut + 1
         self.set_threadid(relative_taskid)
      # for smpss related events, change other task related parameters
      self = self.relativize_smpss_event(first_task_in_cut)    
      return self
      
   # methods to be redefined only by the SMPSs related user events
   def new_phase(self):
      return None
   def get_entering_task(self):
      return None
   def is_exiting_task(self):
      return False
   def is_setting_task_name(self):
      return False
   def is_dependency(self):
      return (False)
   def is_barrier(self):
      return (False)            
   def is_css_finish(self):
      return (False)          
   def dependency_from_whom(self):
      return None
   def relativize_smpss_event(self,first_task_in_cut):
      return self
      
   # methods to be redefined only by blocking MPI calls
   def is_blocking_MPI(self):
      return (False)      
      
      
class UserEvent(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 4)
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.event_type = int (parameters[2])
      self.event_value = int (parameters[3])
      self._tag = tag
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.event_type) + ', ' + str(self.event_value)

   # which phase it start
   def new_phase(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_PHASEID):
         return self.event_value
      else:
         return None

   # which task it enters
   def get_entering_task(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_TASKID):
         if (self.event_value != 0):
            return self.event_value
      return None
   def is_exiting_task(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_TASKID):
         if (self.event_value == 0):
            return True
      return False
      
   def is_setting_task_name(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_TASKNAME_CODED):
         if (self.event_value != 0):
            return True
      return False
   def get_task_name(self):
      assert (self.is_setting_task_name())
      return self.event_value
      
   # getting dependecy information from a user event
   def is_dependency(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_DEPENDENCY):
         return (True)
      else:
         return (False)
   def dependency_from_whom(self):
      assert(self.event_type == SMPSs_user_events.EVENT_TYPE_DEPENDENCY)
      return self.event_value
         
   def is_barrier(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_CSS_BARRIER):
         return (True)
      else:
         return (False)
         
   def is_css_finish(self):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_CSS_START):
         if (self.event_value == 0):
            return (True)
      return False
         
   # change parameters representing tasks
   def relativize_smpss_event(self,first_task_in_cut):
      if (self.event_type == SMPSs_user_events.EVENT_TYPE_TASKID):
         if (self.event_value != 0):
            self.event_value = self.event_value - first_task_in_cut + 1
      elif (self.event_type == SMPSs_user_events.EVENT_TYPE_DEPENDENCY):
         self.event_value = self.event_value - first_task_in_cut + 1
         # filter out the events that represent dependency from a task that is not in the cut
         if (self.event_value <= 0):
            return None
      return self
      
      
class CPUBurst(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 3)
      self._tag = tag      
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.burst_len = float (parameters[2])
   def _pars_string(self):
      return str(self.MPI_process) + ', ' + str(self.threadid) \
             + ', ' + str(self.burst_len)

class BlockBegin(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 3)
      self._tag = tag
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.blockid = int (parameters[2])
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.blockid)

class BlockEnd(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 3)
      self._tag = tag
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.blockid = int (parameters[2])
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.blockid)
   
class NXSend(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 7)
      self._tag = tag
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.dest_MPI_process = int (parameters[2])
      # no specified threadid -> wildcarding
      self.dest_threadid = -2
      self.message_len = int (parameters[3])
      self.msg_tag = int (parameters[4])
      self.communicator = int (parameters[5])
      self.synchronization = int (parameters[6])
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.dest_MPI_process) + ', ' + str(self.dest_threadid) \
             + ', ' + str(self.message_len) + ', ' + str(self.msg_tag) \
             + ', ' + str(self.communicator) + ', ' + str(self.synchronization)
   def is_blocking_MPI(self):
      return (True)                
             
class NXRecv(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 7)
      self._tag = tag
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.src_MPI_process = int (parameters[2])
      # no specified threadid -> wildcarding
      self.src_threadid = -2
      self.message_len = int (parameters[3])
      self.msg_tag = int (parameters[4])
      self.communicator = int (parameters[5])
      self.synchronization = int (parameters[6])
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.src_MPI_process) + ', ' + str(self.src_threadid) \
             + ', ' + str(self.message_len) + ', ' + str(self.msg_tag) \
             + ', ' + str(self.communicator) + ', ' + str(self.synchronization)              
   def is_blocking_MPI(self):
      return (True)           
      
class GlobalOp(TraceRecord):
   def __init__(self, tag = None, *parameters):
      assert(len(parameters) == 8)
      self._tag = tag
      self.MPI_process = int (parameters[0])
      self.threadid = int (parameters[1])
      self.glop_id = int (parameters[2])
      self.comm_id = int (parameters[3])
      self.root_rank = int (parameters[4])
      self.root_thid = int (parameters[5])
      self.bytes_sent = int (parameters[6])
      self.bytes_recvd = int (parameters[7])
      
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.glop_id) + ', ' + str(self.comm_id) \
             + ', ' + str(self.root_rank) + ', ' + str(self.root_thid) \
             + ', ' + str(self.bytes_sent) + ', ' + str(self.bytes_recvd)              
   def is_blocking_MPI(self):
      return (True)         
   
class OutDependency(TraceRecord):
   def __init__(self, MPI_process, src_threadid, dest_threadid):
      self._tag = '"NX send"'
      self.MPI_process = MPI_process
      self.threadid = src_threadid
      self.dest_MPI_process = MPI_process
      self.dest_threadid = dest_threadid
      self.message_len = 1
      if (src_threadid == 0):
         self.msg_tag = DependencyCommunicationTags.DEPENDENCY_TAG_SPAWNING
      elif (dest_threadid == 0):
         self.msg_tag = DependencyCommunicationTags.DEPENDENCY_TAG_JOINING
      else:
         self.msg_tag = DependencyCommunicationTags.DEPENDENCY_TAG_INTERTASK
      self.communicator = 1
      self.synchronization = 2  # asynchronous send
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.dest_MPI_process) + ', ' + str(self.dest_threadid) \
             + ', ' + str(self.message_len) + ', ' + str(self.msg_tag) \
             + ', ' + str(self.communicator) + ', ' + str(self.synchronization)      

class InDependency(TraceRecord):
   def __init__(self, MPI_process, src_threadid, dest_threadid):
      self._tag = '"NX recv"'
      self.MPI_process = MPI_process
      self.threadid = dest_threadid
      self.src_MPI_process = MPI_process
      self.src_threadid = src_threadid
      self.message_len = 1
      if (src_threadid == 0):
         self.msg_tag = DependencyCommunicationTags.DEPENDENCY_TAG_SPAWNING
      elif (dest_threadid == 0):
         self.msg_tag = DependencyCommunicationTags.DEPENDENCY_TAG_JOINING
      else:
         self.msg_tag = DependencyCommunicationTags.DEPENDENCY_TAG_INTERTASK      
      
      self.communicator = 1
      self.synchronization = 2  # asynchronous send
   def _pars_string(self):
      return str(self.MPI_process) + ', ' +  str(self.threadid) \
             + ', ' + str(self.src_MPI_process) + ', ' + str(self.src_threadid) \
             + ', ' + str(self.message_len) + ', ' + str(self.msg_tag) \
             + ', ' + str(self.communicator) + ', ' + str(self.synchronization)  
             
             
   
   
def add_records_for_blocking_task( records_list, current_MPI_process, current_task ):

   # stopping taskid
   new_record = TraceRecord.create_user_event(current_MPI_process, current_task, 
                                       SMPSs_user_events.EVENT_TYPE_TASKID, 0)
   records_list.append(new_record)
   
   # stopping taskname coded   
   new_record = TraceRecord.create_user_event(current_MPI_process, current_task, 
                                       SMPSs_user_events.EVENT_TYPE_TASKNAME_CODED, 0)
   records_list.append(new_record)   
   
   
def add_records_for_unblocking_task( records_list, current_MPI_process, current_task, current_task_code ):
   
   # restarting taskid
   new_record = TraceRecord.create_user_event(current_MPI_process, current_task, 
                                       SMPSs_user_events.EVENT_TYPE_TASKID, current_task)
   records_list.append(new_record)
   
   # restarting taskname coded   
   new_record = TraceRecord.create_user_event(current_MPI_process, current_task, 
                                       SMPSs_user_events.EVENT_TYPE_TASKNAME_CODED, current_task_code)
   records_list.append(new_record)         
   
   