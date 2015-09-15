#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import os
import utils
import input_trf


def start_main_task(trace, process):
   trace.write ('"CPU burst" {%d, 0, 0.000001};;\n' % process)
   trace.write ('"user event" {%d, 0, %d, 1};;\n' % (process, utils.SMPSs_user_events.EVENT_TYPE_TASKID ))
   trace.write ('"user event" {%d, 0, %d, 1};;\n' % (process, utils.SMPSs_user_events.EVENT_TYPE_TASKNAME_CODED))
   trace.write ('"block begin" { %d, 0, 0 };;\n' % (process))
   trace.write ('"block end" { %d, 0, 0 };;\n' % (process))

# restart all the modules and there local states
def start_new_MPI_process(out_trace, new_process):
   start_main_task(out_trace, new_process)


def cut_trace(input_file, 
                          begin_phase, end_phase, 
                          output_file):

   current_MPI_process = utils.STARTING_MPI_PROCESS
     
   #for each record in the trace
   for (record, is_new_MPI_process) in input_trf.trf_file_reader (input_file, begin_phase, end_phase):
      #if starting new MPI process
      if is_new_MPI_process:
         
         # organize and flushout all the collected worker records for this MPI process
         current_MPI_process = current_MPI_process + 1
         start_new_MPI_process(output_file, current_MPI_process)
         sys.stdout.write ("\r")
         sys.stdout.write ("cutting MPI process number:  %d" % current_MPI_process)
         sys.stdout.flush()
         continue     
      
      # DIRTY - force the 0 taskid, and printout the record
      record.threadid = 0
      output_file.write(str (record))

      
   sys.stdout.write ("\n")      
   print "finished cutting"
      
      