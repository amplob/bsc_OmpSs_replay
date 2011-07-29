#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys

def panic (string):
   print string
   sys.exit(1)


#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':

   parse_command_line()
   print 'src file is ', command_line_options.get_source_trace()
   print 'dest file is ', command_line_options.get_destination_trace()  
   
