#! /usr/bin/env python
# -*- coding: utf-8 -*-

import utils


class TranslationState:
   
   def __init__(self):
      self._current_MPI_process = utils.STARTING_MPI_PROCESS
      self._tasks_entered_so_far = 0
      self._tasks_waited_so_far_with_barriers = 0
      self._all_tasks_waited_so_far = []
      self._all_tasks_waited_so_far.sort()
   def start_new_MPI_process(self, new_MPI_process):
      self._current_MPI_process = new_MPI_process
      self._tasks_entered_so_far = 0
      self._tasks_waited_so_far_with_barriers = 0     
      self._all_tasks_waited_so_far = []
      self._all_tasks_waited_so_far.sort()
      
   def get_current_MPI_process(self):
      return self._current_MPI_process      
      
   def get_tasks_entered_so_far(self):
      return self._tasks_entered_so_far
   def set_tasks_entered_so_far(self, tasks):
      if (self._tasks_entered_so_far != tasks - 1):
         utils.panic('failed because tasks_entered_so_far is %d and tasks is %d ' % (self._tasks_entered_so_far, tasks))
      self._tasks_entered_so_far = tasks
      
   def get_tasks_waited_so_far_with_barriers(self):
      return self._tasks_waited_so_far_with_barriers
   def set_tasks_waited_so_far_with_barriers(self, tasks):
      assert (self._tasks_waited_so_far_with_barriers <= tasks)
      self._tasks_waited_so_far_with_barriers = tasks      
      
   def is_already_waited_on(self, task):
      if task in self._all_tasks_waited_so_far:
         return True
      else:
         return False
   def wait_on(self, task):
      self._all_tasks_waited_so_far.append(task)
      self._all_tasks_waited_so_far.sort()
      
      
translation_state = TranslationState()


def start_new_MPI_process(new_MPI_process):
   translation_state.start_new_MPI_process(new_MPI_process)


def add_records_for_blocking_main_task( records_list ):
   current_MPI_process = translation_state.get_current_MPI_process()
   current_task = 0      # main task
   
   utils.add_records_for_blocking_task( records_list, current_MPI_process, current_task)
   
def add_records_for_unblocking_main_task( records_list ):
   
   current_MPI_process = translation_state.get_current_MPI_process()
   current_task = 0      # main task
   current_task_code = 1 # main task
   
   utils.add_records_for_unblocking_task( records_list, current_MPI_process, current_task, current_task_code)
   

def get_caused_main_task_records(record_old):
   
   #assure_record_in_main_task(record_old)
   new_records = []
   if (record_old.get_taskid() == 0):
     
      if (record_old.is_dependency()):
         # this is a wait-on
         dependent_on = record_old.dependency_from_whom()
         if (not translation_state.is_already_waited_on(dependent_on)):
            translation_state.wait_on(dependent_on)
            
            dependency_source = dependent_on
            dependency_destination = 0  # main task
            MPI_process = record_old.get_MPI_process()
            
            dependency_record = utils.TraceRecord.create_in_dependency(MPI_process, dependency_source, dependency_destination)            
            
            add_records_for_blocking_main_task( new_records )
            new_records.append(dependency_record)
            add_records_for_unblocking_main_task( new_records )

      elif (record_old.is_barrier() or record_old.is_css_finish()):
         # this is css barrier or pragma css finish 
         last_task_waited_with_barrier = translation_state.get_tasks_waited_so_far_with_barriers()
         last_task_executed = translation_state.get_tasks_entered_so_far()
         
         add_records_for_blocking_main_task( new_records )
         new_records.append(record_old)
         for task in range(last_task_waited_with_barrier + 1, last_task_executed + 1):
            if (not translation_state.is_already_waited_on(task)):
               translation_state.wait_on(task)
               dependency_source = task
               dependency_destination = 0  # main task
               MPI_process = record_old.get_MPI_process()
               dependency_record = utils.TraceRecord.create_in_dependency(MPI_process, dependency_source, dependency_destination)            
               new_records.append(dependency_record)
         add_records_for_unblocking_main_task( new_records )
         
      elif (record_old.is_blocking_MPI()):
         # some MPI record that may block the task -> do this for pp of paraver trace
         add_records_for_blocking_main_task( new_records )
         new_records.append(record_old)
         add_records_for_unblocking_main_task( new_records )           
         
      else:
         #any other event of the main task
         new_records.append(record_old)       
         
   else:
      #this is for processing worker task records, but obtaining new main task records
      assert (record_old.get_taskid() > 0)
      
      new_task = record_old.get_entering_task()
      if (new_task != None):
         # detected entering a task
         translation_state.set_tasks_entered_so_far(new_task)
         dependency_source = 0 # main task is sending the dependency to spawn the worker
         dependency_destination = new_task
         MPI_process = record_old.get_MPI_process()
         
         dependency_record = utils.TraceRecord.create_out_dependency(MPI_process, dependency_source, dependency_destination)
         new_records.append(dependency_record)

   return (new_records)
   

         