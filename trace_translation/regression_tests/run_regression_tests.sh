#! /bin/bash

echo ''
echo 'running  regression_tests_for_clo_reader.sh'
./regression_tests_for_clo_reader.sh >  output_tests.clo_reader
diff  output_tests.clo_reader  correct_output_tests.clo_reader
echo 'finished  regression_tests_for_clo_reader.sh'  
echo ''
