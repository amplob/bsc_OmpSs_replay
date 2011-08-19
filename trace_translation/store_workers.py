#! /usr/bin/env python
# -*- coding: utf-8 -*-

import utils

PRIORITY_CP_INCREMENT_ORDINARY = 1
PRIORITY_CP_INCREMENT_MPI      = 1000
OUTSTANDING_MPI_PRIORITY       = 0

class WorkerTaskTrace:
   def __init__(self, MPI_process, taskid):
      self._MPI_process = MPI_process
      self._taskid = taskid
      self._records =  []
      self._previous_dependencies = []
      self._has_MPI_activity = False
      self._priority = 1    # the lowes one
      
   def MPI_process(self):
      return self._MPI_process
      
   def get_taskid(self):
      return self._taskid
      
   def get_records(self):
      return self._records
      
   def get_previous_dependencies(self):
      return self._previous_dependencies
      
   def get_priority(self):
      return self._priority
      
      
   def update_priority(self, priority):
      if (priority > self._priority) or (priority == OUTSTANDING_MPI_PRIORITY):
         self._priority = priority
         
   def mark_final_priority(self):
      priority = self._priority
      # make a user event representing the priority
      priority_record = utils.TraceRecord.create_user_event(self._MPI_process, self._taskid,
                                          utils.SMPSs_user_events.EVENT_TYPE_TASK_PRIORITY, priority)
      # put this record at the beginning of the trace of this task
      self._records.insert(0, priority_record)   
      
   def add_first_empty_cpu_burst(self):
      burst_record = utils.TraceRecord.create_empty_CPU_burst(self._MPI_process, self._taskid)
      self._records.insert(0, burst_record)
         
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
         new_worker_task = WorkerTaskTrace(record_MPI_process, record_taskid)
         self._worker_tasks.append(new_worker_task)
      assert (record_taskid <= self._get_worker_tasks_len())
      
      # add record to the 
      this_task = self.get_worker_task(record_taskid)
      records_for_this_task = this_task.get_records()
      records_for_this_task.append(record)


   # remember dependencies among worker tasks
   def add_dependency_among_workers(self, task_before, task_after):
      
      assert (task_before > 0)
      assert (task_before < task_after)
     
      assert (task_after == self._get_worker_tasks_len())
      
      # add this new dependency
      this_task = self.get_worker_task(task_after)
      dependencies_for_this_task = this_task.get_previous_dependencies()
      dependencies_for_this_task.append(task_before)      
      

   # mark which tasks have MPI activity
   def mark_task_with_MPI_activity(self, taskid):
      
      assert (taskid > 0)
      
      # if there is a previous task with MPI activity -> mark the implicit dependency
      if (self._last_task_with_MPI_activity != 0):
         # add the records to mark the implicit dependency among tasks with MPI activity
         task_before = self._last_task_with_MPI_activity
         in_dependency = utils.TraceRecord.create_in_dependency(self._current_MPI_process, task_before, taskid)
         out_dependency = utils.TraceRecord.create_out_dependency(self._current_MPI_process, task_before, taskid)
         self.add_worker_record(in_dependency)
         self.add_worker_record(out_dependency)
         self.add_dependency_among_workers(task_before, taskid)

      # mark that this task has MPI activity
      new_task_with_MPI = self.get_worker_task(taskid)
      new_task_with_MPI.set_has_MPI_activity()
      self._last_task_with_MPI_activity = taskid
            
      
   def flushout_all_worker_records(self, final_trace):
      
      for task in self._worker_tasks:
         this_task_records = task.get_records()
         for record in this_task_records:
            final_trace.write ( str (record))
            
   def flushout_all_intertask_dependencies(self):
      
      for task in self._worker_tasks:
         print "dependencies for task %d:" % task.get_taskid()
         for dep in task.get_previous_dependencies():
            print dep
         
   def flushout_all_tasks_with_MPI(self):
      
      print "these tasks have MPI activity:"
      for task in self._worker_tasks:
         if (task.get_has_MPI_activity()):
            print task.get_taskid()

   def flushout_tasks_priorities(self):
      
      print "TASK PRIORITIES:"
      for task in self._worker_tasks:
         priority = task.get_priority()
         taskid = task.get_taskid()
         print "task %d -> priority %d " % (taskid, priority)


   def calculate_tasks_priorities(self):
      
      # go backwards through all tasks
      for task in reversed (self._worker_tasks):
         
         # update priorities of the tasks that give dependency to the actual task
      
         # if actual task has MPI the increment is PRIORITY_CP_INCREMENT_MPI
         # else the increment is PRIORITY_CP_INCREMENT_ORDINARY     
         if task.get_has_MPI_activity():
            priority_increment = PRIORITY_CP_INCREMENT_MPI
         else:
            priority_increment = PRIORITY_CP_INCREMENT_ORDINARY
         this_task_priority = task.get_priority()
         previous_task_priority = this_task_priority + priority_increment
      
         for dep_taskid in task.get_previous_dependencies():
            dep_task = self.get_worker_task(dep_taskid)
            dep_task.update_priority(previous_task_priority)
      
         # mark the final calculated priority of this task
         if (task.get_has_MPI_activity()):
            this_task_final_priority = OUTSTANDING_MPI_PRIORITY
         else:
            this_task_final_priority = this_task_priority
         task.update_priority(this_task_final_priority)
         task.mark_final_priority()
         
      
   def add_first_empty_cpu_bursts(self):
      for task in self._worker_tasks:
         task.add_first_empty_cpu_burst()
      
      
workers_storage = WorkersStorage()





# interface to store_workers

def start_new_MPI_process(new_MPI_process):
   workers_storage.start_new_MPI_process(new_MPI_process)


def store_worker_record(record):

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
   

def finalize_worker_task_records():
   workers_storage.calculate_tasks_priorities()
   workers_storage.add_first_empty_cpu_bursts()
   

def flushout_all_worker_records(final_trace):
   workers_storage.flushout_all_worker_records(final_trace)



# these are just functions used by the testing scripts
def flushout_all_intertask_dependencies():
   workers_storage.flushout_all_intertask_dependencies()
   
def flushout_all_tasks_with_MPI():
   workers_storage.flushout_all_tasks_with_MPI()   
   
def flushout_tasks_priorities():
   workers_storage.flushout_tasks_priorities()      
         