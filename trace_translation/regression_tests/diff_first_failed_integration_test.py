#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import re
from optparse import OptionParser


def parse_command_line():
   #parsing command line
   usage = "usage: %prog [options]"
   parser = OptionParser(usage=usage)
   parser.add_option("-a", "--action", metavar="name",
                     dest="action",
                     help="what do you want to do with the diff (see or commit)",
                     default="see")
   parser.add_option("-t", "--tests", metavar="name",
                     dest="tests",
                     help="which tests to process (one or all)",
                     default="one")   
                     
   (options, args) = parser.parse_args()   

   return options



def test_correctness (test_command, output_file, output_trace):
   
   test_passed = True
   
   full_command = test_command + ' > ' + output_file
   os.system(full_command)
   

   # check correctness of the output
   correct_output_file = 'correct_outputs/%s' % (os.path.basename(output_file))
   diff_output_command = 'diff %s %s > check.out 2> check.err' % (output_file, correct_output_file)
   if (os.system(diff_output_command)):
      test_passed = False
      
   # check correctness of the resulting trace
   if (output_trace != None):
      if (os.path.exists(output_trace)):
         correct_output_trace_file = 'correct_translated_trf_files/%s' % (os.path.basename(output_trace))
         diff_trace_command = 'diff %s %s > check.out 2> check.err' % (output_trace, correct_output_trace_file)
         if (os.system(diff_trace_command)):
            test_passed = False      
   
   if (test_passed):
      tests_summary.add_one_correct_test(full_command)
   else:
      tests_summary.add_one_failed_test(full_command)



def get_outputs_to_compare(test):

   ## to collect arguments of the test
   binary = None
   s_file = None
   d_file = None
   correct_d_file = None
   begin = None
   end = None
   output = None
   correct_output = None
   simulation = None
   correct_simulation = None
   
   binary_re = re.compile(r'^(\S*).*')
   matched = re.findall(binary_re, test)
   assert matched
   binary = matched[0]
   
   source_re = re.compile(r'(-s\s+)(\S+).*')
   matched = re.findall(source_re, test)
   if matched:
      s_file = matched[0][1]
      
   dest_re = re.compile(r'(-d\s+)(\S+).*')
   matched = re.findall(dest_re, test)
   if matched:
      d_file = matched[0][1]         
      
   begin_re = re.compile(r'(-b\s+)(\d+).*')
   matched = re.findall(begin_re, test)
   if matched:
      begin = matched[0][1]         
   
   end_re = re.compile(r'(-e\s+)(\d+).*')
   matched = re.findall(end_re, test)
   if matched:
      end = matched[0][1]            
      
   output_re = re.compile(r'(>\s*)(\S+)')
   matched = re.findall(output_re, test)
   if matched:
      output = matched[0][1]       

   
   trace_basename = os.path.basename(d_file)   
   dimemas_output = 'Dimemas_simulations/' + os.path.splitext(trace_basename)[0] + '.dim.out'
   print "resulting simulation file: " + dimemas_output
   
   correct_dimemas_output = 'correct_translated_trf_files/' + os.path.basename(dimemas_output) 
   print "correct trace file: " + correct_dimemas_output
   
   return (dimemas_output, correct_dimemas_output)
   

   

#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':
   
   regression_output = open ("regression_output", 'r')
   regression_output = regression_output.readlines()
   
   options = parse_command_line()
   
   
   action_is_see = False
   action_is_commit = False
   only_one_test = True
   
   if (options.action == 'commit'):
      action_is_commit = True
   else:
      action_is_see = True
      
      
   if (options.tests == 'all'):
      only_one_test = False
      
   
   for line in regression_output:
      if ('total failed tests' in line):
         failed_tests = line.split(':')[1]
         print "total tests failed: ", failed_tests
      if (line.startswith('.')):
         print "first tests with error is :"
         print line 
         
         (dim_out, correct_dim_out) = get_outputs_to_compare(line)

         print "diff of the dimemas simulations"
         if (action_is_see):
            os.system('diff %s %s' % (dim_out, correct_dim_out))
         else:
            os.system('cp %s %s'  % (dim_out, correct_dim_out))
            print "commited"

         
         if (only_one_test):
            sys.exit(1)
   
   print "all tests exited succesfully"
   
   