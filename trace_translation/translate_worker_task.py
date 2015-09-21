#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import utils




class TranslationState:
   
   def __init__(self):
      self._current_MPI_process = utils.STARTING_MPI_PROCESS
      self._tasks_entered_so_far = 0
      self._currently_in_task = False
      self._current_task_type = 0
   def start_new_MPI_process(self, new_MPI_process):
      self._current_MPI_process = new_MPI_process
      self._tasks_entered_so_far = 0
      self._currently_in_task = False
      self._current_task_type = 0
      
   def get_current_MPI_process(self):
      return self._current_MPI_process
   def get_tasks_entered_so_far(self):
      return self._tasks_entered_so_far
   def _set_tasks_entered_so_far(self, tasks):
      if (self._tasks_entered_so_far != tasks - 1):
         utils.panic('failed because tasks_entered_so_far is %d and tasks is %d ' % (self._tasks_entered_so_far, tasks))
      self._tasks_entered_so_far = tasks
      
   def enter_task(self, taskid):
      assert (not self._currently_in_task)
      self._currently_in_task = True
      self._set_tasks_entered_so_far(taskid)
      self._current_task_type = 0
   def exit_task(self):
      assert (self._currently_in_task)
      self._currently_in_task = False
      self._current_task_type = 0
      
   def is_currently_in_task(self):
      return self._currently_in_task
      
   def set_current_task_type(self, task_type):
      self._current_task_type = task_type  
   def get_current_task_type(self):
      return self._current_task_type

      
translation_state = TranslationState()


def start_new_MPI_process(new_MPI_process):
   translation_state.start_new_MPI_process(new_MPI_process)
   
def add_records_for_blocking_task( records_list ):
   current_MPI_process = translation_state.get_current_MPI_process()
   current_task = translation_state.get_tasks_entered_so_far()
   
   utils.add_records_for_blocking_task( records_list, current_MPI_process, current_task)

   
def add_records_for_unblocking_task( records_list ):
   
   current_MPI_process = translation_state.get_current_MPI_process()
   current_task = translation_state.get_tasks_entered_so_far()
   current_task_code = translation_state.get_current_task_type()
   
   utils.add_records_for_unblocking_task( records_list, current_MPI_process, current_task, current_task_code)
   
   

def get_caused_worker_task_records(record_old):
   
   #assure_record_in_main_task(record_old)
   new_records = []
   if (record_old.get_taskid() != 0):
     
      #this is for processing worker task records, and obtaining final worker task records
      assert (record_old.get_taskid() > 0)
      
      new_task = record_old.get_entering_task()
      if (new_task != None):
         # detected entering a task
         assert (not translation_state.is_currently_in_task())               
         translation_state.enter_task(new_task)
         dependency_source = 0 # main task is sending the dependency to spawn the worker
         dependency_destination = new_task
         MPI_process = record_old.get_MPI_process()
         
         
         dependency_record = utils.TraceRecord.create_in_dependency(MPI_process, dependency_source, dependency_destination)
         new_records.append(dependency_record)
         new_records.append(record_old)
         
      elif (record_old.is_exiting_task()):
         # detected exiting a task
         assert (translation_state.is_currently_in_task())               
         dependency_source =  translation_state.get_tasks_entered_so_far() 
         dependency_destination = 0  # to the main task
         MPI_process = record_old.get_MPI_process()
         
         dependency_record = utils.TraceRecord.create_out_dependency(MPI_process, dependency_source, dependency_destination)
         new_records.append(dependency_record)

         # add block_end for this task
         taskname_coded = translation_state.get_current_task_type()
         block_id = taskname_coded + utils.TASKS_BLOCKS_OFFSET
         current_task = translation_state.get_tasks_entered_so_far()  
         block_end_record = utils.TraceRecord.create_block_end(MPI_process, current_task, block_id)
         new_records.append(block_end_record)
         
         # add the user event signaling the exit from the task
         new_records.append(record_old)
         translation_state.exit_task()
         
      elif (record_old.is_setting_task_name()):
         # detected setting taskname
         assert (translation_state.is_currently_in_task())
         taskname_coded = record_old.get_task_name()
         translation_state.set_current_task_type(taskname_coded)
         
         new_records.append(record_old)
         
         # add block_begin for this task
         block_id = taskname_coded + utils.TASKS_BLOCKS_OFFSET
         MPI_process = record_old.get_MPI_process()
         current_task = translation_state.get_tasks_entered_so_far()  
         block_begin_record = utils.TraceRecord.create_block_begin(MPI_process, current_task, block_id)
         new_records.append(block_begin_record)
         
      elif (record_old.is_dependency()):
         # this is a dependency among tasks
         assert (translation_state.is_currently_in_task())      
         dependent_on = record_old.dependency_from_whom()

         dependency_source = dependent_on
         dependency_destination = translation_state.get_tasks_entered_so_far()  
         MPI_process = record_old.get_MPI_process()
         
         dependency_in_record = utils.TraceRecord.create_in_dependency(MPI_process, dependency_source, dependency_destination)
         dependency_out_record = utils.TraceRecord.create_out_dependency(MPI_process, dependency_source, dependency_destination)

         add_records_for_blocking_task(new_records)         
         new_records.append(dependency_in_record)
         add_records_for_unblocking_task(new_records)         
         new_records.append(dependency_out_record)
         new_records.append(record_old)
         
      elif (record_old.is_blocking_MPI()):
         # some MPI record that may block the task -> do this for pp of paraver trace
         #print "here is a blocking MPI in a task"
         add_records_for_blocking_task( new_records )
         new_records.append(record_old)
         add_records_for_unblocking_task( new_records )           
      
      else:
         new_records.append(record_old)

   return (new_records)
   

         