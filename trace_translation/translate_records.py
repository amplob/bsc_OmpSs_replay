#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import os
import utils
import input_trf

import translate_main_task

import translate_worker_task
import store_workers


def start_main_task(trace, process):
   trace.write ('"CPU burst" {%d, 0, 0.0};;\n' % process)
   trace.write ('"user event" {%d, 0, %d, 1};;\n' % (process, utils.SMPSs_user_events.EVENT_TYPE_TASKID ))
   trace.write ('"user event" {%d, 0, %d, 1};;\n' % (process, utils.SMPSs_user_events.EVENT_TYPE_TASKNAME_CODED))

# restart all the modules and there local states
def start_new_MPI_process(out_trace, new_process):
   start_main_task(out_trace, new_process)
   translate_main_task.start_new_MPI_process(new_process)
   translate_worker_task.start_new_MPI_process(new_process)
   store_workers.start_new_MPI_process(new_process)


def flush_main_task_records(out_trace, records):
   for record in records:
      out_trace.write(str (record))
      

def store_worker_records(records):
   for record in records:
      store_workers.store_worker_record(record)

def flush_finalized_worker_task_records(out_trace):
   store_workers.finalize_worker_task_records()
   store_workers.flushout_all_worker_records(out_trace)      


# interface to other files

def translate_all_records(input_file, 
                          begin_phase, end_phase, 
                          output_file):

   current_MPI_process = utils.STARTING_MPI_PROCESS
   
   #for each record in the trace
   for (record, is_new_MPI_process) in input_trf.trf_file_reader (input_file, begin_phase, end_phase):
      #if starting new MPI process
      if is_new_MPI_process:
         # organize and flushout all the collected worker records for this MPI process
         flush_finalized_worker_task_records(output_file)
         current_MPI_process = current_MPI_process + 1
         start_new_MPI_process(output_file, current_MPI_process)
         sys.stdout.write ("\r")
         sys.stdout.write ("translating MPI process %d" % current_MPI_process)
         sys.stdout.flush()
         continue     
      
      # there is a useful record to process
      if record.is_blocking_MPI():
         print 'translate_all_records  -> MPI call  ' ,  str(record)
      
      # records for main task -> directly put them to the output trace
      new_main_task_records = translate_main_task.get_caused_main_task_records(record)
      flush_main_task_records(output_file, new_main_task_records)
      
      # records for worker tasks -> store them, and when the MPI process finished, orgranize them and flush to the output trace
      new_worker_task_records = translate_worker_task.get_caused_worker_task_records(record)
      store_worker_records(new_worker_task_records)   
      
      
   # to organize and flushout worker tasks for the last MPI process
   flush_finalized_worker_task_records(output_file)      
      
   sys.stdout.write ("\n")      
   print "finished translation"
      
      