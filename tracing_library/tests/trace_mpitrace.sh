#!/bin/bash

#export LD_LIBRARY_PATH=/opt/osshpc/mpich-gm/64/lib/shared:${LD_LIBRARY_PATH}
#export LD_PRELOAD=/gpfs/apps/CEPBATOOLS/mpitrace/64/lib/libmpitrace.so
export LD_PRELOAD=/gpfs/apps/CEPBATOOLS/extrae/latest/mpich/64/lib/libmpitrace.so
export MPITRACE_ON=1
#export LD_PRELOAD=/gpfs/home/bsc18/bsc18422/valgrind/auxprogs/libmpiwrap-PPC64_LINUX.so


./mpi_probe $@ >trfxxxxx$SLURM_PROCID.out 2>trfxxxxx$SLURM_PROCID.err


unset LD_PRELOAD
