
import utils

###################################################
#for parsing the command line options
###################################################
from optparse import OptionParser

def parse_command_line():
   #parsing command line
   usage = "usage: %prog [options]"
   parser = OptionParser(usage=usage)
   parser.add_option("-s", "--source-trace", metavar="name",
                     dest="src_trace_name",
                     help="source: MPI trace with SMPSs related events (has to be old format)")
   parser.add_option("-d", "--destination-trace", metavar="name",
                     dest="dest_trace_name",
                     help="result: MPI/SMPSs trace (also old format)")                     
   parser.add_option("-b", "--beginning-phase", metavar="INT",
                     dest="begin_phase",
                     help="From which phase to cut the trace")
   parser.add_option("-e", "--ending-phase", metavar="INT",
                     dest="end_phase",
                     help="Until which phase to cut the trace")
   (options, args) = parser.parse_args()   

   # check the avalibility of the files
   if ((not options.src_trace_name) or (not options.dest_trace_name)):
      utils.panic ("incorrect usage (source or dest trace not specified), check:    %prog --help")
     
   # convert the phase numbers into integers
   try:      
      if options.begin_phase == None:
         begin_phase = -1
      else:
         begin_phase = int (options.begin_phase)
      
      if options.end_phase == None:
         end_phase = -1
      else:
         end_phase = int (options.end_phase)   
   except ValueError:
      utils.panic ("Failed to convert begin or end phase into integer")
      
   #check sanity of the phase   
   if (options.begin_phase and options.end_phase):
      if (int (options.begin_phase) >= int (options.end_phase)):
         utils.panic('ERROR in phase selection:  end_phase <= begin_phase,  so the cut is empty')
         
   if (options.begin_phase):
      if (int (options.begin_phase) < 0):
         utils.panic('ERROR in phase selection:  start_phase cannot be negative number')
   if (options.end_phase):
      if (int (options.end_phase) < 0):
         utils.panic('ERROR in phase selection:  end_phase cannot be negative number')         
   
   #return phase in integer types, with -1 meaning that the phase is not specified
   options.begin_phase = begin_phase
   options.end_phase = end_phase
            
   return options
