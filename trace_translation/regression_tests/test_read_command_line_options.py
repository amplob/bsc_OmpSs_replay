#! /usr/bin/env python
# -*- coding: utf-8 -*-

#from read_command_line_options import parse_command_line
import read_command_line_options
from read_command_line_options import command_line_options

#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':

   read_command_line_options.parse_command_line()
   print 'src file is ', command_line_options.get_source_trace()
   print 'dest file is ', command_line_options.get_destination_trace()  
   print 'start phase is ', command_line_options.get_start_phase()
   print 'end phase is ', command_line_options.get_end_phase()     
   
