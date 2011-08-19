#! /usr/bin/env python
# -*- coding: utf-8 -*-

#from read_command_line_options import parse_command_line
import read_command_line_options

#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':

   clo_options = read_command_line_options.parse_command_line()
   print 'src file is ', clo_options.src_trace_name
   print 'dest file is ', clo_options.dest_trace_name
   print 'start phase is ', clo_options.begin_phase
   print 'end phase is ', clo_options.end_phase
   
