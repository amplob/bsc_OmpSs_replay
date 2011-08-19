#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os

#from read_command_line_options import parse_command_line
import read_command_line_options

import utils
import input_trf
import store_workers
import translate_worker_task


def copy_header(input_file, output_file):
   
   line = input_file.readline()
   while '"CPU burst" { ' not in line:
      output_file.write(line)
      line = input_file.readline()

   ## unread the last line
   input_file.seek(-len(line), os.SEEK_CUR) 


def is_record_in_main_task(record):
   if (record.get_taskid() == 0 ):
      return True
   else:
      return False

def start_new_MPI_process(new_process):
   translate_worker_task.start_new_MPI_process(new_process)
   store_workers.start_new_MPI_process(new_process)

def flush_worker_task_records(out_trace):
   store_workers.flushout_all_worker_records(out_trace)         
   store_workers.flushout_all_intertask_dependencies()   
   store_workers.flushout_all_tasks_with_MPI()   
   

#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':
   
   clo_options = read_command_line_options.parse_command_line()   
   
   try:
      input_file = open(clo_options.src_trace_name, 'r')
      output_file = open(clo_options.dest_trace_name, 'w')
   except IOError:
      print "These files do not exist, exiting gracefully"
   
   begin_phase = int (clo_options.begin_phase)
   end_phase = int (clo_options.end_phase)
   
   copy_header(input_file, output_file)
   
   current_MPI_process = utils.STARTING_MPI_PROCESS
   
   #for each record in the trace
   for (record, is_new_MPI_process) in input_trf.trf_file_reader (input_file, begin_phase, end_phase):
      #if starting new MPI process
      if is_new_MPI_process:
         flush_worker_task_records(output_file)
         print "caught new_MPI_process in test_store_workers"
         current_MPI_process = current_MPI_process + 1
         start_new_MPI_process(current_MPI_process)
         continue      
      
      new_worker_task_records = translate_worker_task.get_caused_worker_task_records(record)
      for new_record in new_worker_task_records:
         store_workers.store_worker_records (new_record)
         print new_record
         
   
   # to flushout the last MPI process
   flush_worker_task_records(output_file)

      
   #store_workers.flush_worker_records ()

  