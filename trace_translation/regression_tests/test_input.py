#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os

#from read_command_line_options import parse_command_line
import read_command_line_options

#from read_command_line_options import parse_command_line
import input_trf

import utils


def copy_header(input_file, output_file):
   
   line = input_file.readline()
   while '"CPU burst" { ' not in line:
      output_file.write(line)
      line = input_file.readline()

   ## unread the last line
   input_file.seek(-len(line), os.SEEK_CUR) 

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
   
   # go through all records and translate them
   current_MPI_process = utils.STARTING_MPI_PROCESS
   EOF = False   
   first_in_MPI_process = True
   
   for (record, new_MPI_process) in input_trf.trf_file_reader (input_file, begin_phase, end_phase):
      if new_MPI_process:
         print "caught new_MPI_process in translate_all_records"     
         first_in_MPI_process = True
      else:
         output_file.write(str(record) + '\n')
         if (first_in_MPI_process):
            print "first record in MPI process is ", str(record)
            first_in_MPI_process = False
   

  