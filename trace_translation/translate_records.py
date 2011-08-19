#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import utils
import input_trf

class TranslationState:
   
   def __init__(self):
      current_MPI_process = utils.STARTING_MPI_PROCESS
      tasks_executed_so_far = 0
      tasks_waited_so_far = 0
   

def translate_all_records(input_file, 
                          begin_phase, end_phase, 
                          output_file):

   EOF = False
   
   translation_state = TranslationState()
      
      
   #for each record in the trace
   for (record, is_new_MPI_process) in input_trf.trf_file_reader (input_file, begin_phase, end_phase):
      #if starting new MPI process
      if is_new_MPI_process:
         #flushout worker tasks collected for the actual process
         
         #start new MPI process (reset state)         
         #record is None, the next record is taken in the next call to trf_file_reader
         print "caught new_MPI_process in translate_all_records"
         continue      
      
      # there is a useful record to process
      #print_caused_main_task_records(record)
      (new_main_task_records, translation_state) = get_caused_main_task_records(record, translation_state)
      printout_final_records(new_main_task_records)
      
      #store_caused_worker_task_records(record)
      (new_worker_task_records, translation_state) = get_caused_main_task_records(record, translation_state)
      store_final_worker_records(new_worker_task_records)
      
   print "finished translate_all_records"
      
      