#! /bin/bash

cd tests

echo '----------------------------starting test_taskname_coder'
./test_taskname_coder > output.test_taskname_coder
if [ $(diff output.test_taskname_coder default.test_taskname_coder | wc -l) -gt 0 ]; then
  echo "test failed"
fi
echo '------------------------------ending test_taskname_coder'


echo '----------------------------starting test_dep_calculator'
./test_dep_calculator > output.test_dep_calculator
diff output.test_dep_calculator default.test_dep_calculator
echo '------------------------------ending test_taskname_coder'


export MPTRACE_CONFIG_FILE=./mpitrace.xml
export LD_PRELOAD=/home/vladimir/work/mpitrace-install/lib/
# /gpfs/scratch/bsc18/bsc18422/mpitrace/extrae-2.0-linux-ppc64-mpichmx-papi-3.6.2/lib/libmpitrace.so
export MPITRACE_ON=1

echo '----------------------------starting mpi_probe'
mpirun -n 1 mpi_probe > output.mpi_probe
diff output.mpi_probe default.mpi_probe
echo '------------------------------ending test_taskname_coder'

