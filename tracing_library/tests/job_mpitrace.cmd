#! /bin/bash
# @ initialdir = .
# @ output = unb.out
# @ error =  unb.err
# @ total_tasks = 4
# @ cpus_per_task = 1
# @ tasks_per_node = 4
# @ wall_clock_limit =  200
# @ features = mx

#/gpfs/apps/CEPBATOOLS/mpitrace-devel/64

#export LD_LIBRARY_PATH=/gpfs/apps/PAPI/3.2.1-970mp/64/lib:${LD_LIBRARY_PATH}
export MPTRACE_CONFIG_FILE=./mpitrace.xml

time srun trace_mpitrace.sh
