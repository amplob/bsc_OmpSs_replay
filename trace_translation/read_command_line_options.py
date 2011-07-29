
import utils

###################################################
#for parsing the command line options
###################################################
from optparse import OptionParser

class Command_line:
   def __init__(self):
      self.start_phase_defined = False
      self.end_phase_defined = False
      
   def set_source_trace(self, file_name):
      self.src_filename = file_name
   def get_source_trace(self):
      return self.src_filename
   def set_destination_trace(self, file_name):
      self.dest_filename = file_name
   def get_destination_trace(self):
      return self.dest_filename      
      
   def define_beginning_phase(self, phaseID):
      assert (self.start_phase_defined == False)
      assert (phaseID >= 0)
      self.start_phase_defined = True
      self.start_phase = phaseID
   def define_ending_phase(self, phaseID):
      assert (self.end_phase_defined == False)
      assert (phaseID >= 0)
      self.end_phase_defined = True
      self.end_phase = phaseID   
   def assure_cut_sanity(self):
      if (self.start_phase_defined and self.end_phase_defined):
         if (self.end_phase <= self.start_phase):
            utils.panic('ERROR in phase selection:  end_phase <= begin_phase,  so the cut is empty')
         
   def is_start_phase_defined(self):
      return self.start_phase_defined
   def is_end_phase_defined(self):
      return self.end_phase_defined      
   def get_start_phase(self):
      if (not self.start_phase_defined):
         utils.panic("ERROR: cannot call command_line_options.get_start_phase because start phase is not defined")
      return self.start_phase
   def get_end_phase(self):
      if (not self.end_phase_defined):
         utils.panic("ERROR: cannot call command_line_options.get_end_phase because end phase is not defined")      
      return self.end_phase      
      
command_line_options = Command_line()



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
   else:
      command_line_options.set_source_trace(options.src_trace_name)
      command_line_options.set_destination_trace(options.dest_trace_name)
     
   
       
   if (options.begin_phase):
      command_line_options.define_beginning_phase(int (options.begin_phase))
   if (options.end_phase):
      command_line_options.define_ending_phase(int (options.end_phase))
   command_line_options.assure_cut_sanity()
      
   return options
