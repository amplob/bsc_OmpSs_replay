#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from read_command_line_options import parse_command_line
from translate_records import translate_all_records

def copy_header(input_file, output_file):
    line = input_file.readline()
    assert (line.startswith('#DIMEMAS'))
    output_file.write(trf_header)

trf_header = '''
SDDFA;;

#1:
"CPU burst" {
  int      "taskid";
  int      "thid";
  double   "time";
};;


#2:
"NX send" {
  int      "taskid";
  int      "thid";
  int      "dest taskid";
  int      "msg length";
  int      "tag";
  int      "commid";
  int      "synchronism";
};;


#3:
"NX recv" {
  int      "taskid";
  int      "thid";
  int      "source taskid";
  int      "msg length";
  int      "tag";
  int      "commid";
  int      "type";
};;


#40:
"block begin" {
  int      "taskid";
  int      "thid";
  int      "blockid";
};;


#41:
"block end" {
  int      "taskid";
  int      "thid";
  int      "blockid";
};;


#50:
"user event" {
  int      "taskid";
  int      "thid";
  int      "type";
  int      "value";
};;


#42:
"block definition" {
  int      "block_id";
  char     "block_name"[];
  char     "activity_name"[];
  int      "src_file";
  int      "src_line";
};;


#43:
"file definition" {
  int      "file_id";
  char     "location"[];
};;


#100:
"communicator definition" {
  int     "comm_id";
  int     "comm_size";
  int     "global_ranks"[];
};;


#200:
"global OP definition" {
  int     "glop_id";
  char     "glop_name"[];
};;


#48:
"user event type definition" {
  int     "type";
  char    "name"[];
  int     "color";
};;


#49:
"user event value definition" {
  int     "type";
  int     "value";
  char    "name"[];
};;


#201:
"global OP" {
  int     "rank";
  int     "thid";
  int     "glop_id";
  int     "comm_id";
  int     "root_rank";
  int     "root_thid";
  int     "bytes_sent";
  int     "bytes_recvd";
};;


#500:
"1sided OP definition" {
  int     "1sided_op_id";
  char    "1sided_op_name"[];
};;


#501:
"1sided OP" {
  int     "taskid";
  int     "thid";
  int     "1sided_op_id";
  int     "handle";
  int     "tgt_thid";
  int     "msg_size";
};;



"block definition" {   1, "MPI_Allgather", "MPI", 0, 0 };;
"block definition" {   2, "MPI_Allgatherv", "MPI", 0, 0 };;
"block definition" {   3, "MPI_Allreduce", "MPI", 0, 0 };;
"block definition" {   4, "MPI_Alltoall", "MPI", 0, 0 };;
"block definition" {   5, "MPI_Alltoallv", "MPI", 0, 0 };;
"block definition" {   6, "MPI_Barrier", "MPI", 0, 0 };;
"block definition" {   7, "MPI_Bcast", "MPI", 0, 0 };;
"block definition" {   8, "MPI_Gather", "MPI", 0, 0 };;
"block definition" {   9, "MPI_Gatherv", "MPI", 0, 0 };;
"block definition" {  10, "MPI_Op_create", "MPI", 0, 0 };;
"block definition" {  11, "MPI_Op_free", "MPI", 0, 0 };;
"block definition" {  12, "MPI_Reduce_scatter", "MPI", 0, 0 };;
"block definition" {  13, "MPI_Reduce", "MPI", 0, 0 };;
"block definition" {  14, "MPI_Scan", "MPI", 0, 0 };;
"block definition" {  15, "MPI_Scatter", "MPI", 0, 0 };;
"block definition" {  16, "MPI_Scatterv", "MPI", 0, 0 };;
"block definition" {  17, "MPI_Attr_delete", "MPI", 0, 0 };;
"block definition" {  18, "MPI_Attr_get", "MPI", 0, 0 };;
"block definition" {  19, "MPI_Attr_put", "MPI", 0, 0 };;
"block definition" {  20, "MPI_Comm_create", "MPI", 0, 0 };;
"block definition" {  21, "MPI_Comm_dup", "MPI", 0, 0 };;
"block definition" {  22, "MPI_Comm_free", "MPI", 0, 0 };;
"block definition" {  23, "MPI_Comm_group", "MPI", 0, 0 };;
"block definition" {  24, "MPI_Comm_rank", "MPI", 0, 0 };;
"block definition" {  25, "MPI_Comm_remote_group", "MPI", 0, 0 };;
"block definition" {  26, "MPI_Comm_remote_size", "MPI", 0, 0 };;
"block definition" {  27, "MPI_Comm_size", "MPI", 0, 0 };;
"block definition" {  28, "MPI_Comm_split", "MPI", 0, 0 };;
"block definition" {  29, "MPI_Comm_test_inter", "MPI", 0, 0 };;
"block definition" {  30, "MPI_Comm_compare", "MPI", 0, 0 };;
"block definition" {  31, "MPI_Group_difference", "MPI", 0, 0 };;
"block definition" {  32, "MPI_Group_excl", "MPI", 0, 0 };;
"block definition" {  33, "MPI_Group_free", "MPI", 0, 0 };;
"block definition" {  34, "MPI_Group_incl", "MPI", 0, 0 };;
"block definition" {  35, "MPI_Group_intersection", "MPI", 0, 0 };;
"block definition" {  36, "MPI_Group_rank", "MPI", 0, 0 };;
"block definition" {  37, "MPI_Group_range_excl", "MPI", 0, 0 };;
"block definition" {  38, "MPI_Group_range_incl", "MPI", 0, 0 };;
"block definition" {  39, "MPI_Group_size", "MPI", 0, 0 };;
"block definition" {  40, "MPI_Group_translate_ranks", "MPI", 0, 0 };;
"block definition" {  41, "MPI_Group_union", "MPI", 0, 0 };;
"block definition" {  42, "MPI_Group_compare", "MPI", 0, 0 };;
"block definition" {  43, "MPI_Intercomm_create", "MPI", 0, 0 };;
"block definition" {  44, "MPI_Intercomm_merge", "MPI", 0, 0 };;
"block definition" {  45, "MPI_Keyval_free", "MPI", 0, 0 };;
"block definition" {  46, "MPI_Keyval_create", "MPI", 0, 0 };;
"block definition" {  47, "MPI_Abort", "MPI", 0, 0 };;
"block definition" {  48, "MPI_Error_class", "MPI", 0, 0 };;
"block definition" {  49, "MPI_Errhandler_create", "MPI", 0, 0 };;
"block definition" {  50, "MPI_Errhandler_free", "MPI", 0, 0 };;
"block definition" {  51, "MPI_Errhandler_get", "MPI", 0, 0 };;
"block definition" {  52, "MPI_Error_string", "MPI", 0, 0 };;
"block definition" {  53, "MPI_Errhandler_set", "MPI", 0, 0 };;
"block definition" {  54, "MPI_Finalize", "MPI", 0, 0 };;
"block definition" {  55, "MPI_Get_processor_name", "MPI", 0, 0 };;
"block definition" {  56, "MPI_Init", "MPI", 0, 0 };;
"block definition" {  57, "MPI_Initialized", "MPI", 0, 0 };;
"block definition" {  58, "MPI_Wtick", "MPI", 0, 0 };;
"block definition" {  59, "MPI_Wtime", "MPI", 0, 0 };;
"block definition" {  60, "MPI_Address", "MPI", 0, 0 };;
"block definition" {  61, "MPI_Bsend", "MPI", 0, 0 };;
"block definition" {  62, "MPI_Bsend_init", "MPI", 0, 0 };;
"block definition" {  63, "MPI_Buffer_attach", "MPI", 0, 0 };;
"block definition" {  64, "MPI_Buffer_detach", "MPI", 0, 0 };;
"block definition" {  65, "MPI_Cancel", "MPI", 0, 0 };;
"block definition" {  66, "MPI_Request_free", "MPI", 0, 0 };;
"block definition" {  67, "MPI_Recv_init", "MPI", 0, 0 };;
"block definition" {  68, "MPI_Send_init", "MPI", 0, 0 };;
"block definition" {  69, "MPI_Get_count", "MPI", 0, 0 };;
"block definition" {  70, "MPI_Get_elements", "MPI", 0, 0 };;
"block definition" {  71, "MPI_Ibsend", "MPI", 0, 0 };;
"block definition" {  72, "MPI_Iprobe", "MPI", 0, 0 };;
"block definition" {  73, "MPI_Irecv", "MPI", 0, 0 };;
"block definition" {  74, "MPI_Irsend", "MPI", 0, 0 };;
"block definition" {  75, "MPI_Isend", "MPI", 0, 0 };;
"block definition" {  76, "MPI_Issend", "MPI", 0, 0 };;
"block definition" {  77, "MPI_Pack", "MPI", 0, 0 };;
"block definition" {  78, "MPI_Pack_size", "MPI", 0, 0 };;
"block definition" {  79, "MPI_Probe", "MPI", 0, 0 };;
"block definition" {  80, "MPI_Recv", "MPI", 0, 0 };;
"block definition" {  81, "MPI_Rsend", "MPI", 0, 0 };;
"block definition" {  82, "MPI_Rsend_init", "MPI", 0, 0 };;
"block definition" {  83, "MPI_Send", "MPI", 0, 0 };;
"block definition" {  84, "MPI_Sendrecv", "MPI", 0, 0 };;
"block definition" {  85, "MPI_Sendrecv_replace", "MPI", 0, 0 };;
"block definition" {  86, "MPI_Ssend", "MPI", 0, 0 };;
"block definition" {  87, "MPI_Ssend_init", "MPI", 0, 0 };;
"block definition" {  88, "MPI_Start", "MPI", 0, 0 };;
"block definition" {  89, "MPI_Startall", "MPI", 0, 0 };;
"block definition" {  90, "MPI_Test", "MPI", 0, 0 };;
"block definition" {  91, "MPI_Testall", "MPI", 0, 0 };;
"block definition" {  92, "MPI_Testany", "MPI", 0, 0 };;
"block definition" {  93, "MPI_Test_cancelled", "MPI", 0, 0 };;
"block definition" {  94, "MPI_Testsome", "MPI", 0, 0 };;
"block definition" {  95, "MPI_Type_commit", "MPI", 0, 0 };;
"block definition" {  96, "MPI_Type_contiguous", "MPI", 0, 0 };;
"block definition" {  97, "MPI_Type_extent", "MPI", 0, 0 };;
"block definition" {  98, "MPI_Type_free", "MPI", 0, 0 };;
"block definition" {  99, "MPI_Type_hindexed", "MPI", 0, 0 };;
"block definition" { 100, "MPI_Type_hvector", "MPI", 0, 0 };;
"block definition" { 101, "MPI_Type_indexed", "MPI", 0, 0 };;
"block definition" { 102, "MPI_Type_lb", "MPI", 0, 0 };;
"block definition" { 103, "MPI_Type_size", "MPI", 0, 0 };;
"block definition" { 104, "MPI_Type_struct", "MPI", 0, 0 };;
"block definition" { 105, "MPI_Type_ub", "MPI", 0, 0 };;
"block definition" { 106, "MPI_Type_vector", "MPI", 0, 0 };;
"block definition" { 107, "MPI_Unpack", "MPI", 0, 0 };;
"block definition" { 108, "MPI_Wait", "MPI", 0, 0 };;
"block definition" { 109, "MPI_Waitall", "MPI", 0, 0 };;
"block definition" { 110, "MPI_Waitany", "MPI", 0, 0 };;
"block definition" { 111, "MPI_Waitsome", "MPI", 0, 0 };;
"block definition" { 112, "MPI_Cart_coords", "MPI", 0, 0 };;
"block definition" { 113, "MPI_Cart_create", "MPI", 0, 0 };;
"block definition" { 114, "MPI_Cart_get", "MPI", 0, 0 };;
"block definition" { 115, "MPI_Cart_map", "MPI", 0, 0 };;
"block definition" { 116, "MPI_Cart_rank", "MPI", 0, 0 };;
"block definition" { 117, "MPI_Cart_shift", "MPI", 0, 0 };;
"block definition" { 118, "MPI_Cart_sub", "MPI", 0, 0 };;
"block definition" { 119, "MPI_Cartdim_get", "MPI", 0, 0 };;
"block definition" { 120, "MPI_Dims_create", "MPI", 0, 0 };;
"block definition" { 121, "MPI_Graph_get", "MPI", 0, 0 };;
"block definition" { 122, "MPI_Graph_map", "MPI", 0, 0 };;
"block definition" { 123, "MPI_Graph_neighbors", "MPI", 0, 0 };;
"block definition" { 124, "MPI_Graph_create", "MPI", 0, 0 };;
"block definition" { 125, "MPI_Graphdims_get", "MPI", 0, 0 };;
"block definition" { 126, "MPI_Graph_neighbors_count", "MPI", 0, 0 };;
"block definition" { 127, "MPI_Topo_test", "MPI", 0, 0 };;
"block definition" { 128, "TRACE_ON", "DimemasTrace_API", 0, 0 };;
"block definition" { 129, "I/O Read", "I/O", 0, 0 };;
"block definition" { 130, "I/O Write", "I/O", 0, 0 };;
"block definition" { 131, "I/O Call", "I/O", 0, 0 };;
"block definition" { 132, "MPI_Win_create", "MPI", 0, 0 };;
"block definition" { 133, "MPI_Win_free", "MPI", 0, 0 };;
"block definition" { 134, "MPI_Put", "MPI", 0, 0 };;
"block definition" { 135, "MPI_Get", "MPI", 0, 0 };;
"block definition" { 136, "MPI_Accumulate", "MPI", 0, 0 };;
"block definition" { 137, "MPI_Win_fence", "MPI", 0, 0 };;
"block definition" { 138, "MPI_Win_complete", "MPI", 0, 0 };;
"block definition" { 139, "MPI_Win_start", "MPI", 0, 0 };;
"block definition" { 140, "MPI_Win_post", "MPI", 0, 0 };;
"block definition" { 141, "MPI_Win_wait", "MPI", 0, 0 };;
"block definition" { 142, "MPI_Win_test", "MPI", 0, 0 };;
"block definition" { 143, "MPI_Win_lock", "MPI", 0, 0 };;
"block definition" { 144, "MPI_Win_unlock", "MPI", 0, 0 };;
"block definition" { 145, "MPI_Init_thread", "MPI", 0, 0 };;
"block definition" { 146, "LAPI_Init", "LAPI", 0, 0 };;
"block definition" { 147, "LAPI_Term", "LAPI", 0, 0 };;
"block definition" { 148, "LAPI_Put", "LAPI", 0, 0 };;
"block definition" { 149, "LAPI_Get", "LAPI", 0, 0 };;
"block definition" { 150, "LAPI_Fence", "LAPI", 0, 0 };;
"block definition" { 151, "LAPI_Gfence", "LAPI", 0, 0 };;
"block definition" { 152, "LAPI_Address_init", "LAPI", 0, 0 };;
"block definition" { 153, "LAPI_Amsend", "LAPI", 0, 0 };;
"block definition" { 154, "LAPI_Rmw", "LAPI", 0, 0 };;
"block definition" { 155, "LAPI_Waitcntr", "LAPI", 0, 0 };

"global OP definition" { 0, "MPI_Barrier" };;
"global OP definition" { 1, "MPI_Bcast" };;
"global OP definition" { 2, "MPI_Gather" };;
"global OP definition" { 3, "MPI_Gatherv" };;
"global OP definition" { 4, "MPI_Scatter" };;
"global OP definition" { 5, "MPI_Scatterv" };;
"global OP definition" { 6, "MPI_Allgather" };;
"global OP definition" { 7, "MPI_Allgatherv" };;
"global OP definition" { 8, "MPI_Alltoall" };;
"global OP definition" { 9, "MPI_Alltoallv" };;
"global OP definition" {10, "MPI_Reduce" };;
"global OP definition" {11, "MPI_Allreduce" };;
"global OP definition" {12, "MPI_Reduce_Scatter" };;
"global OP definition" {13, "MPI_Scan" };;


'''


#######################################################
#######################################################
#main
#######################################################
#######################################################
if __name__ == '__main__':

   clo_options = parse_command_line()
   
   print ('----------------------------------------------------------------------------------------------------------')
   print (' translating MPI trace to MPI/SMPSs trace')
   print ('----------------------------------------------------------------------------------------------------------')
   print ('input MPI trace         :    ', clo_options.src_trace_name)
   print ('output MPI/SMPSs trace  :    ', clo_options.dest_trace_name)
   if (clo_options.begin_phase != -1): 
      print ('start phase             :    ', clo_options.begin_phase)
   if (clo_options.end_phase != -1):
      print ('end phase               :    ', clo_options.end_phase)
   print ('----------------------------------------------------------------------------------------------------------')
   
   
   try:
      input_file = open(clo_options.src_trace_name, 'r')
      output_file = open(clo_options.dest_trace_name, 'w')
   except IOError:
      print ("Failed to open source or destination file")
   
   
   copy_header(input_file, output_file)
   
   translate_all_records(input_file, 
                         clo_options.begin_phase, clo_options.end_phase, 
                         output_file)

