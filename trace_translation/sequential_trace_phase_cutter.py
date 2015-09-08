#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from read_command_line_options import parse_command_line
from cut_MPI_trace import cut_trace

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

   clo_options = parse_command_line()
   
   print '----------------------------------------------------------------------------------------------------------'
   print ' cutting MPI trace'
   print '----------------------------------------------------------------------------------------------------------'
   print 'input MPI trace         :    ', clo_options.src_trace_name
   print 'output MPI/SMPSs trace  :    ', clo_options.dest_trace_name
   if (clo_options.begin_phase != -1): 
      print 'start phase             :    ', clo_options.begin_phase
   if (clo_options.end_phase != -1):
      print 'end phase               :    ', clo_options.end_phase
   print '----------------------------------------------------------------------------------------------------------'        
   
   
   try:
      input_file = open(clo_options.src_trace_name, 'r')
      output_file = open(clo_options.dest_trace_name, 'w')
   except IOError:
      print "Failed to open source or destination file"
   
   
   copy_header(input_file, output_file)
   
   cut_trace(input_file, 
             clo_options.begin_phase, clo_options.end_phase, 
             output_file)

