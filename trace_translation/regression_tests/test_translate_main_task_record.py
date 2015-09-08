#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os

#from read_command_line_options import parse_command_line
import read_command_line_options

#from read_command_line_options import parse_command_line
import input_trf
import translate_main_task 

import utils

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
         print "caught new_MPI_process in translate_all_records"
         current_MPI_process = current_MPI_process + 1
         translate_main_task.start_new_MPI_process(current_MPI_process)
         continue      
      
      new_main_task_records = translate_main_task.get_caused_main_task_records(record)
      for printing_record in new_main_task_records:
         print printing_record
      


  