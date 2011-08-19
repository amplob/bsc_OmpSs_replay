#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
from optparse import OptionParser

def parse_command_line():
   #parsing command line
   usage = "usage: %prog [options]"
   parser = OptionParser(usage=usage)
   parser.add_option("-t", "--tests", metavar="name",
                     dest="tests",
                     help="which tests to process (some or all) - some -> only one that do not have # in front",
                     default="some")   
   (options, args) = parser.parse_args()   
   return options

class TestsSummary:
   def __init__(self):
      self._total_tests = 0
      self._failed_tests = 0
      self._failed_tests_list = []
      self._correct_tests = 0
      self._correct_tests_list = []      
   def add_one_correct_test(self, test):
      self._total_tests = self._total_tests + 1
      self._correct_tests = self._correct_tests + 1
      self._correct_tests_list.append(test)
   def add_one_failed_test(self, test):
      self._total_tests = self._total_tests + 1
      self._failed_tests = self._failed_tests + 1
      self._failed_tests_list.append(test)
   def print_tests_summary(self):
      print ""
      print "--------------------------------------------"
      print "------summary of regression testing---------"
      print "--------------------------------------------"
      print ""
      summary_file = open ("regression_output", 'w')
      print " total tests:             ", self._total_tests
      summary_file.write (" total tests: %d\n" % self._total_tests)

      print " total failed tests:      ", self._failed_tests
      summary_file.write (" total failed tests: %d\n" % self._failed_tests)
      
      print " percentage of success: ", (float (self._correct_tests) / float(self._total_tests) * 100)
      summary_file.write("percentage of success: %f\n" % (float (self._correct_tests) / float(self._total_tests) * 100))
      if (self._failed_tests > 0):
         summary_file.write ( "failed tests:\n")
         for test in self._failed_tests_list:
            summary_file.write(test + '\n')
            
      print ""             
      print "--------------------------------------------"
     

tests_summary = TestsSummary()


#def create_MN_file (trace_file):
   
   ## generate the name of the MN file
   #basename = os.path.basename(trace_file)
   #basename = os.path.splitext(basename)[0]    
   #MN_name = 'Dimemas_simulations/MN_' + basename + '.cfg'
   #MN_out = open (MN_name, 'w')   
   
   
   ## generate the content of the MN file
   
   ## how many MPI processes
   #processes_re = re.compile(r'(\d+)MPI.*')
   #matched = re.findall(processes_re, trace_file)
   #if matched:
      #MPI_processes = int ( matched[0])
   
   #base_MN = open ("MN_1_1cpu.cfg", 'r')
   #lines = base_MN.readlines()
   #for line in lines:
      #if line.startswith('"environment information" {"'):
         ## line for environment information - change number of MPI processes
         #new_line = '"environment information" {"", 0, "", %d, 0.0, 0, 1};;\n' % MPI_processes
         #MN_out.write (new_line)
      #elif line.startswith('"node information" {0'):
         ## lines with nodes information
         #for node in range (0, MPI_processes):
            #new_line = ('"node information" {%d, 0, "", 4, 1, 1, 0.0, 0.0, 1.0, 0.0, 0.0};;\n' % node)
            #MN_out.write (new_line)
      #elif line.startswith('"mapping information" {"'):
         ## mapping information -> change trace file to simulate
         #new_line = '"mapping information" {"%s", 1, [1] {0}};;\n' % trace_file
         #MN_out.write (new_line)
      #else:
         #MN_out.write (line)
   
   #MN_out.close()
   #return MN_name
   
   

def test_correctness (test_command, output_file, output_trace):
   
   test_passed = True
   correct_output_trace_file = None
   MN_file = None
   
   full_command = test_command + ' > ' + output_file
   os.system(full_command)
   

   # check correctness of the output
   correct_output_file = 'correct_outputs/%s' % (os.path.basename(output_file))
   diff_output_command = 'diff %s %s > check.out 2> check.err' % (output_file, correct_output_file)
   if (os.system(diff_output_command)):
      test_passed = False
      
   ## check correctness of the resulting trace
   #if (output_trace != None):
      #if (os.path.exists(output_trace)):
         #correct_output_trace_file = 'correct_translated_trf_files/%s' % (os.path.basename(output_trace))
         #diff_trace_command = 'diff %s %s > check.out 2> check.err' % (output_trace, correct_output_trace_file)
         #if (os.system(diff_trace_command)):
            #test_passed = False
            
   ## check correctness of the Dimemas simulation
   #if (output_trace != None):
      #if (os.path.exists(output_trace)):
         
         ## make trace of the new format
         #basename = os.path.basename(output_trace)
         #trace_new_format = "Dimemas_simulations/new_format_" + basename
         #os.system('trf2trf %s %s' % (output_trace, trace_new_format))
         #MN_file_name = create_MN_file (trace_new_format)
         #basename = os.path.basename(output_trace)
         #prv_file = 'Dimemas_simulations/' + os.path.splitext(basename)[0] + '.prv'
         #dimemas_output = 'Dimemas_simulations/' + os.path.splitext(basename)[0] + '.dim.out'
         #simulation_command = 'Dimemas3 -S 32000  -pa %s %s > %s' % (prv_file, MN_file_name, dimemas_output)
         #correct_dimemas_output = os.path.basename(dimemas_output)
         #correct_dimemas_output = 'correct_Dimemas_simulations/' + correct_dimemas_output
         
         #os.system(simulation_command)
         #diff_dimemas_output = 'diff %s %s > check.out 2> check.err' % (dimemas_output, correct_dimemas_output)
         #if (os.system(diff_dimemas_output)):
            #test_passed = False


   if (test_passed):
      tests_summary.add_one_correct_test(full_command)
   else:
      tests_summary.add_one_failed_test(full_command)

#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':
   
   process_all_tests = False
   options = parse_command_line()
   if (options.tests == "all"):
      process_all_tests = True
   
   
   os.system('rm resulting_outputs/*')
   os.system('rm resulting_trf_files/*')
   
   tests_file = open ("unit_tests.txt", 'r')
   all_unit_tests = tests_file.readlines()
   
   input_traces_file = open("input_traces_list.txt", 'r')
   all_traces = input_traces_file.readlines()
   

   for test in all_unit_tests:
      # skip the empty line
      if test == '\n':
         continue
      # skip commented tests
      if (not process_all_tests):
         if (test.startswith('#')):
            continue
      
      test = test.lstrip('# ')
      
      # to collect arguments of the test
      binary = None
      s_file = None
      d_file = None
      begin = None
      end = None
      
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


      # now to execute the tests
      cut_options = ''
      test_command_cut_options = ''
      if (begin != None):
         test_command_cut_options = test_command_cut_options + '  ' + '-b %s' % begin
         cut_options = cut_options + '-b-%s' % begin
      if (end != None):
         test_command_cut_options = test_command_cut_options + '  ' + '-e %s' % end
         cut_options = cut_options + '-e-%s' % end         
     
      if (s_file != "SPEC.all_input_traces"):
         
         # not passing through all traces - use the specific names 
         test_command = binary
         basename = os.path.basename(os.path.splitext(binary)[0])
         options = basename
         if (s_file != None):
            test_command = test_command + '  ' + '-s %s' % s_file
            basename = os.path.basename(s_file)
            basename = os.path.splitext(basename)[0] 
            options = options + '-s-%s' % basename
         if (d_file != None):
            test_command = test_command + '  ' + '-d %s' % d_file
            basename = os.path.basename(d_file)
            basename = os.path.splitext(basename)[0]             
            options = options + '-d-%s' % basename      
    
         options = options + cut_options
         test_command = test_command + test_command_cut_options
    
         output_file = 'resulting_outputs/%s.out' % (options)
         
         test_correctness (test_command, output_file, d_file)
         
      else:
         # now passing through all traces
         for in_trace in all_traces:
            
            # skip commented traces
            if (not process_all_tests):
               if (in_trace.startswith('#')):
                  continue            
               
            in_trace = in_trace.lstrip('# ')               
            in_trace = in_trace.strip('\n')
            
            test_command = binary
            bin_basename = os.path.basename(os.path.splitext(binary)[0])
            options = bin_basename
            
            s_file = 'input_traces_folder/' + in_trace
            test_command = test_command + '  ' + '-s %s' % s_file            
            s_basename = os.path.basename(s_file)
            s_basename = os.path.splitext(s_basename)[0]              
            options = options + '-s-%s' % s_basename
            
            d_file = 'resulting_trf_files/' + bin_basename + '-' + s_basename + cut_options + '.trf'
            test_command = test_command + '  ' + '-d %s' % d_file
            basename = os.path.basename(d_file)
            options = options + '-d-%s' % basename               
            
            options = options + cut_options
            test_command = test_command + test_command_cut_options
         
            output_file = 'resulting_outputs/%s.out' % (options)
            
            test_correctness (test_command, output_file, d_file)
                        
   tests_summary.print_tests_summary()    