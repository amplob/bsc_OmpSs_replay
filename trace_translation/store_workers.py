#! /usr/bin/env python
# -*- coding: utf-8 -*-

import utils

PRIORITY_CP_INCREMENT_ORDINARY = 1
PRIORITY_CP_INCREMENT_MPI      = 1000
OUTSTANDING_MPI_PRIORITY       = 0

class WorkerTaskTrace:
   def __init__(self, taskid):
      self.taskid = taskid
      self.records =  []
      self.previous_dependencies = []
      self._has_MPI_activity = False
      self._priority = 1    # the lowes one
      
   def update_priority(self, priority):
      if (priority > self._priority) or (priority == OUTSTANDING_MPI_PRIORITY):
         self._priority = priority
         
   def set_has_MPI_activity(self):
      self._has_MPI_activity = True
      
   def get_has_MPI_activity(self):
      return self._has_MPI_activity
         
      
      

class WorkersStorage:
   
   def __init__(self):
      self._current_MPI_process = utils.STARTING_MPI_PROCESS
      self._worker_tasks = []
      self._last_task_with_MPI_activity = 0
      # for each task has a list of the tasks that it has to wait dependency from
   def start_new_MPI_process(self, new_MPI_process):
      self._current_MPI_process = new_MPI_process
      self._worker_tasks = []
      self._last_task_with_MPI_activity = 0
      
   def get_worker_task(self, taskid):
      return self._worker_tasks[taskid - 1]
   def get_current_MPI_process(self):
      return self._current_MPI_process
      
   def _get_worker_tasks_len(self):
      return len (self._worker_tasks)    
      
   # add record to the appropriate list
   def add_worker_record(self, record):
      
      record_MPI_process = record.get_MPI_process()
      assert (record_MPI_process == self._current_MPI_process)
      record_taskid = record.get_taskid()
      assert (record_taskid > 0)
      workers_list_len = self._get_worker_tasks_len()
      
      # if needs enlargement of the list of worker tasks
      if (record_taskid > workers_list_len):
         assert (record_taskid == workers_list_len + 1)
         new_worker_task = WorkerTaskTrace(record_taskid)
         self._worker_tasks.append(new_worker_task)
      assert (record_taskid <= self._get_worker_tasks_len())
      
      # add record to the 
      this_task = self.get_worker_task(record_taskid)
      records_for_this_task = this_task.records
      records_for_this_task.append(record)


   # remember dependencies among worker tasks
   def add_dependency_among_workers(self, task_before, task_after):
      
      assert (task_before > 0)
      assert (task_before < task_after)
     
      assert (task_after == self._get_worker_tasks_len())
      
      # add this new dependency
      this_task = self.get_worker_task(task_after)
      dependencies_for_this_task = this_task.previous_dependencies
      dependencies_for_this_task.append(task_before)      
      

   # mark which tasks have MPI activity
   def mark_task_with_MPI_activity(self, taskid):
      
      assert (taskid > 0)
      
      # if there is a previous task with MPI activity -> mark the implicit dependency
      if (self._last_task_with_MPI_activity != 0):
         # add the records to mark the implicit dependency among tasks with MPI activity
         in_dependency = utils.TraceRecord.create_in_dependency(self._current_MPI_process, self._last_task_with_MPI_activity, taskid)
         out_dependency = utils.TraceRecord.create_out_dependency(self._current_MPI_process, self._last_task_with_MPI_activity, taskid)
         self.add_worker_record(in_dependency)
         self.add_worker_record(out_dependency)

      # mark that this task has MPI activity
      new_task_with_MPI = self.get_worker_task(taskid)
      new_task_with_MPI.set_has_MPI_activity()
      self._last_task_with_MPI_activity = taskid
            
      
   def flushout_all_worker_records(self, final_trace):
      
      for task in self._worker_tasks:
         for record in task.records:
            final_trace.write ( str (record))
            
   def flushout_all_intertask_dependencies(self):
      
      for task in self._worker_tasks:
         print "dependencies for task %d:" % task.taskid
         for dep in task.previous_dependencies:
            print dep
         
   def flushout_all_tasks_with_MPI(self):
      
      print "these tasks have MPI activity:"
      for task in self._worker_tasks:
         if (task.get_has_MPI_activity()):
            print task.taskid

   def flushout_task_priorities(self):
      
      print "TASK PRIORITIES:"
      for task in range (0, len(self._task_priorities)):
         print "task %d -> priority %d " % (task + 1, priority)

   # make a user event to specify priority and put it as the first record in the task
   def _set_priority(self, taskid, priority):
      priority_record = utils.TraceRecord.create_user_event(self._current_MPI_process, taskid,
                                          utils.SMPSs_user_events.EVENT_TYPE_TASK_PRIORITY, priority)
      worker_task = self.get_worker_task(taskid)
      worker_task_records = worker_task.records
      worker_task_records.insert(0, priority_record)

   #def calculate_tasks_priorities(self):
      
      #current_MPI_process = self._current_MPI_process
      
      ##print "calculate_tasks_priorities"
      #for taskid in reversed ( range (0 ,len (self._worker_tasks))):
         
         #print 'calculate_tasks_priorities for task ' , taskid
         
         #task = self._worker_tasks[taskid]
         
         #if (task.has_MPI_activity):
            #print "priority is infinite (has_MPI_activity)"
            #self._set_priority(taskid)
            ## calculate_tasks_priorities based on critical path
            #for dep_taskid in task.previous_dependencies:
               #dep_task = self._worker_tasks[dep_taskid]
               #dep_task.update_priority (task.priority, PRIORITY_CP_INCREMENT_ORDINARY)
               #dep_task_current_priority = dep_task.priority
               ##if (dep_task_current_priority < )
               
         
         #print "priority is  ", task.priority
         

      
   def add_first_empty_cpu_bursts(self):
      pass
      
workers_storage = WorkersStorage()





# interface to store_workers

def start_new_MPI_process(new_MPI_process):
   workers_storage.start_new_MPI_process(new_MPI_process)


def store_worker_records(record):

   if record.is_dependency():
      # this is a depedency among tasks -> coded as user event -> so this user event I can omit
      from_whom = record.dependency_from_whom()
      record_taskid = record.get_taskid()
      workers_storage.add_dependency_among_workers (from_whom, record_taskid)
   elif record.is_blocking_MPI():
      record_taskid = record.get_taskid()
      workers_storage.mark_task_with_MPI_activity(record_taskid)
   else:
      workers_storage.add_worker_record(record)
   

def flushout_all_worker_records(final_trace):
   #workers_storage.calculate_tasks_priorities()
   #workers_storage.add_first_empty_cpu_bursts()   
   workers_storage.flushout_all_worker_records(final_trace)

def flushout_all_intertask_dependencies():
   workers_storage.flushout_all_intertask_dependencies()
   
def flushout_all_tasks_with_MPI():
   workers_storage.flushout_all_tasks_with_MPI()   
   
def flushout_task_priorities():
   workers_storage.flushout_task_priorities()      
   
   
         