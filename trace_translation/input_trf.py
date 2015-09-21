#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import utils
from utils import SMPSs_user_events, TraceRecord



class SMPSsState:
   def __init__(self, begin_phase, end_phase):
      self._begin_phase = begin_phase
      self._end_phase = end_phase
      self._current_MPI_process = utils.STARTING_MPI_PROCESS
   def __str__(self):
      line = (
               'current SMPSs state is :\n'
               '---------------------------------------------------\n'
               'MPI_process is          ' + str(self._current_MPI_process) + '\n'
               'is in cut               ' + str(self._currently_in_cut) + '\n'
               'first task in the cut   ' + str(self._first_task_in_cut) + '\n'
               'is currently in task    ' + str(self._currently_in_task) + '\n'
               'current absolute task   ' + str(self._current_absolute_task) + '\n'
               '---------------------------------------------------\n')
      return line
   # initialization of starting a new MPI process
   def start_new_MPI_process(self, new_MPI):
      assert(new_MPI == self._current_MPI_process + 1)
      self._current_MPI_process = new_MPI
      self._first_task_in_cut = None
      self._currently_in_cut = False   
      self._currently_in_task= False
      self._current_absolute_task = None
      if (self._begin_phase <= 0):
         self._currently_in_cut = True
         
   # access functions
   def get_current_MPI_process(self):
      return self._current_MPI_process
   def is_currently_in_cut(self):
      return self._currently_in_cut
   def is_currently_in_task(self):
      return self._currently_in_task
   def get_current_absolute_task(self):
      return self._current_absolute_task
   def get_first_task_in_cut(self):
      return self._first_task_in_cut
   def set_first_task_in_cut(self, taskid):
      self._first_task_in_cut = taskid
      
   # cut manipulations   
   def enter_new_phase(self, new_phase):
      if (self._currently_in_cut):
         # is it exiting the cut
         if (new_phase == self._end_phase):
            self._currently_in_cut = False
      else:
         # is it entering the cut
         if (new_phase == self._begin_phase):
            self._currently_in_cut = True
            
   # entering/exiting tasks
   def enter_new_task(self, new_task):
      assert(not self._currently_in_task)
      # if already defined
      if (self._current_absolute_task):
         assert(new_task == (self._current_absolute_task + 1))
      self._current_absolute_task = new_task
      self._currently_in_task = True
      if (self._first_task_in_cut == None):
         self._first_task_in_cut = self._current_absolute_task
   def exit_task(self):
      # exiting task is signalized with 0
      assert(self._currently_in_task)
      self._currently_in_task = False
          

      
      
      
      
def trf_file_reader (input_file, begin_phase, end_phase):
   
   current_state = SMPSsState(begin_phase, end_phase)

   for line in input_file:
      
      # translate line into trace record class
      record = TraceRecord.create_input_record(line)
      if record == None:
         return
         # utils.panic('cannot translate line into record,    line is:   %s' % (line))

############################################################################
## manage start_new_MPI_process
############################################################################

      # management of MPI processes, reinitialization for starting new MPI process
      MPI_process_now = record.get_MPI_process()
      if (MPI_process_now > current_state.get_current_MPI_process() ):
         current_state.start_new_MPI_process (MPI_process_now)
         yield (None, True)
 
############################################################################
## cut managemet
############################################################################ 
 
      # is this a start of the new phase, and should it enter/leave the cut
      new_phase = record.new_phase()
      if new_phase:
         current_state.enter_new_phase(new_phase)

      #if not in cut, just get a new record
      if not current_state.is_currently_in_cut():
         continue      

############################################################################
## to relativize the trace, put parameters relative to cut
############################################################################

      # handle state when entering tasks
      absolute_taskid = record.get_entering_task()
      if absolute_taskid:
         current_state.enter_new_task(absolute_taskid)
   
      # relativize the record, start counting from the 1 relatively in the cut
      # now if I have first_task_in_cut I can relativize, if not, it should not be relativized at all
      first_task_in_cut = current_state.get_first_task_in_cut()
      if (first_task_in_cut != None):
         is_now_in_task = current_state.is_currently_in_task()
         current_absolute_task = current_state.get_current_absolute_task()
         record = record.relativize_inside_cut(first_task_in_cut, is_now_in_task, current_absolute_task)
         if (record == None):
            # this record is lost due to cut filtering, go to the next record
            continue

      # handle state when exiting tasks
      leaving_task = record.is_exiting_task()
      if leaving_task:
         current_state.exit_task()   

############################################################################
## deliver one useful record
############################################################################

      yield (record, False)
      
      
      