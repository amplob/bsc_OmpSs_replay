#! /bin/bash

echo ''
echo '   running test  ./test_read_command_line_options.py'
./test_read_command_line_options.py

echo ''
echo '   running test  ./test_read_command_line_options.py -s tests/trf.trf '
./test_read_command_line_options.py -s tests/trf.trf

echo '' 
echo '   running test  ./test_read_command_line_options.py -d tests/trf_smpss.trf '
./test_read_command_line_options.py -d tests/trf_smpss.trf 

echo ''
echo '   running test  ./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf '
./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf 
 
echo ''
echo '   running test  ./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf  -b 6 -e 3'
./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf  -b 6 -e 3

echo ''
echo '   running test  ./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf  -b 2 -e 7'
./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf  -b 2 -e 7

echo ''
echo '   running test  ./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf  -b 6 -e 6'
./test_read_command_line_options.py -s tests/trf.trf  -d tests/trf_smpss.trf  -b 6 -e 6

