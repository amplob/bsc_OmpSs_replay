#! /bin/bash

# here are the names that are going to be used in the tests
export TASKNAMES=/home/vladimir/work/codes/MPISSTrace/MPISSTrace/tracing_library/tests/tasknames.txt

cd tests

echo '----------------------------starting test_taskname_coder'
./test_taskname_coder > output.test_taskname_coder
diff output.test_taskname_coder default.test_taskname_coder
#if [ $(diff output.test_taskname_coder default.test_taskname_coder | wc -l) -gt 0 ]; then
#  echo "test failed"
#fi
echo '------------------------------ending test_taskname_coder'


echo '----------------------------starting test_dep_calculator'
./test_dep_calculator > output.test_dep_calculator
diff output.test_dep_calculator default.test_dep_calculator
echo '------------------------------ending test_dep_calculator'


echo '----------------------------starting test_ssvalgrindcc_interface'
./test_ssvalgrindcc_interface > output.test_ssvalgrindcc_interface
diff output.test_ssvalgrindcc_interface default.test_ssvalgrindcc_interface
echo '------------------------------ending test_ssvalgrindcc_interface'



echo '----------------------------starting test_integration_no_tasks'
./test_integration_no_tasks > output.test_integration_no_tasks
diff output.test_integration_no_tasks default.test_integration_no_tasks
echo '------------------------------ending test_integration_no_tasks'


echo '----------------------------starting test_integration_with_tasks'
./test_integration_with_tasks > output.test_integration_with_tasks
diff output.test_integration_with_tasks default.test_integration_with_tasks
echo '------------------------------ending test_integration_with_tasks'

# export MPTRACE_CONFIG_FILE=./mpitrace.xml
# export LD_PRELOAD=/home/vladimir/work/mpitrace-install/lib/
# export MPITRACE_ON=1
# 
# echo '----------------------------starting mpi_probe'
# mpirun -n 1 mpi_probe > output.mpi_probe
# diff output.mpi_probe default.mpi_probe
# echo '------------------------------ending test_taskname_coder'

