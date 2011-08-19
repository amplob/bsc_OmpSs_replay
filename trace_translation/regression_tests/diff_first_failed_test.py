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
   parser.add_option("-f", "--files", metavar="name",
                     dest="files",
                     help="which files to compare (traces, outputs, simulations or all)",
                     default="all")
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

   print "resulting output file: " +  output
   if (output != None):
      correct_output = 'correct_outputs/' + os.path.basename(output) 
   if (correct_output != None):
      print "correct output file: " +  correct_output
   else:
      print "correct output file: " +  "NO OUTPUT FILE"
   
   #if (d_file != None):
      #print "resulting trace file: " + d_file
   #else:
      #print "resulting trace file: " + "NO OUTPUT TRACE FILE"
   #if (d_file != None):
      #correct_d_file = 'correct_translated_trf_files/' + os.path.basename(d_file) 
   #if (correct_d_file != None):
      #print "correct trace file: " + correct_d_file
   #else:
      #print "correct trace file: " + "NO OUTPUT TRACE FILE"
      
      
   #if (d_file != None):
      
      #print "resulting simulation file: " + d_file
   #else:
      #print "resulting trace file: " + "NO OUTPUT TRACE FILE"
   #if (d_file != None):
      #correct_d_file = 'correct_translated_trf_files/' + os.path.basename(d_file) 
   #if (correct_d_file != None):
      #print "correct trace file: " + correct_d_file
   #else:
      #print "correct trace file: " + "NO OUTPUT TRACE FILE"      
   
   return (output, correct_output)
   

   

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
   operate_on_traces = False
   operate_on_outputs = False
   only_one_test = True
   
   if (options.action == 'commit'):
      action_is_commit = True
   else:
      action_is_see = True
      
   if (options.files == 'traces'):
      operate_on_traces = True
   elif (options.files == 'outputs'):
      operate_on_outputs = True
   else:
      operate_on_traces = True
      operate_on_outputs = True
      
   if (options.tests == 'all'):
      only_one_test = False
      
   
   for line in regression_output:
      if ('total failed tests' in line):
         failed_tests = line.split(':')[1]
         print "total tests failed: ", failed_tests
      if (line.startswith('./')):
         print "first tests with error is :"
         print line 
         
         (output, correct_output ) = get_outputs_to_compare(line)
         
         if (output != None):
            print "diff of the output files"
            if (operate_on_outputs):
               if (action_is_see):
                  os.system('diff %s %s' % (output, correct_output))
               else:
                  os.system('cp %s %s'  % (output, correct_output))
                  print "commited"
         
         #if (output_trf != None):
            #print "diff of the traces"
            #if (operate_on_traces):
               #if (action_is_see):
                  #os.system('diff %s %s' % (output_trf, correct_trf))
               #else:
                  #os.system('cp %s %s'  % (output_trf, correct_trf))
                  #print "commited"
         
         if (only_one_test):
            sys.exit(1)
   
   print "all tests exited succesfully"
   
   