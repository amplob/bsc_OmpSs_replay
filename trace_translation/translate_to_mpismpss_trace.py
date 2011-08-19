#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import utils
from read_command_line_options import parse_command_line
from translate_records import translate_all_records

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
   print 'src file is ', clo_options.src_trace_name
   print 'dest file is ', clo_options.dest_trace_name
   print 'start phase is ', clo_options.begin_phase
   print 'end phase is ', clo_options.end_phase
   
   
   try:
      input_file = open(clo_options.src_trace_name, 'r')
      output_file = open(clo_options.dest_trace_name, 'w')
   except IOError:
      print "Failed to open source or destination file"
   
   copy_header(input_file, output_file)
   
   translate_all_records(input_file, 
                         clo_options.begin_phase, clo_options.end_phase, 
                         output_file)
   

