#!/usr/bin/env python3
# -*- coding: utf-8 -*-


from optparse import OptionParser
import os
from subprocess import call
import shutil
import difflib
import glob

#################################################################################
### GLOBAL NAMES - MOSTLY FILENAMES
#################################################################################

# helper directories
#from backend.src.utils import util_fncs

DIR_INTERMEDIATE_FILES = '.test_intermediate'
DIR_OUTPUT_FILES = '.'
DIR_DIMEMAS_SIMULATIONS = '.tmp_folder_dimemas_simulations'
DIR_PARAMEDIR_PROCESSING = '.tmp_folder_paramedir_processing'

# prefixes for the filenames used
# llvm instrumentation log
INSTRUMENTATION_LOG_PREFIX = 'instrumentation_log'
# translated indexed logs
DEEP_LOG_OBJECTS_PREFIX = 'log_objects_deep'
DEEP_LOG_EXECUTION_PREFIX = 'log_execution_deep_fast'
# logs related to a specific task decomposition
OBJECTS_SELECTED_TYPES_PREFIX = 'log_object_types_selected'
OBJECTS_SELECTED_INSTANCES_PREFIX = 'log_object_instances_selected'
PROGRAM_STRUCTURE_SELECTED_PREFIX = 'log_program_structure'
TASKS_LIST_PREFIX = 'tasks_list_test_'
TASKS_SELECTED_PREFIX = 'tasks_log'
CUM_OBJ_USAGE_PREFIX = 'log_cum_objects_usage'
OBJ_USAGE_TASKLIST_PREFIX = 'log_object_usage_tasklist'
OBJ_USAGE_DEPLIST_PREFIX = 'log_object_usage_deplist'
DEPENDENCIES_PREFIX = 'log_dependencies'
DIMEMAS_TRACE_PREFIX = 'trf_superscalar0'
PCF_ADDITION_PREFIX = 'pcf_superscalar'
TASKS_COSTS_PREFIX = 'tasks_costs'
PARAVER_TRACE_TMP = 'prv.prv'
DIMEMAS_TRACE_NEW_FORMAT_TMP = 'dim.dim'
XDOT_GRAPH_PREFIX = 'graph_dependencies'
REALIZED_TASKS_PREFIX = 'realized_tasks_log'
DRIVER_INPUT_TESTS_PREFIX = 'tests_driver_'
BOTTLENECKS_INPUT_TESTS_PREFIX = 'tests_bottlenecks_'
NEXT_DECOMPOSITION_PREFIX = 'next_decomposition_configuration'
DRIVER_OUTPUT_PREFIX = 'output_run_driver'
OBJECTS_OFFSETS_PREFIX = 'objects_offsets'
OBJECTS_FRAGMENTATION_PREFIX = 'objects_fragmentation'



# binaries used
BINARIES = {
    #'TAREADOR_CLANG': 'tareador-clang',
    'GCC'           : 'gcc',
    'VALGRIND_BIN' : 'valgrind',
    'TAREADOR_MAKE_DEEP_LOG': 'make_fast_deep_log.py',
    'TAREADOR_SELECT_DECOMP': 'select_decomposition.py',
    'TAREADOR_BOTTLENECK_TASKS': 'bottleneck_tasks.py',
    'TRF2TRF_BIN': 'trf2trf',
    'DIMEMAS_SIMULATION_SCRIPT': 'dimemas_simulation_ncores.py'
}

#################################################################################
### UTILS
#################################################################################

def get_immediate_subdirectories(dir):
    return [name for name in os.listdir(dir)
            if os.path.isdir(os.path.join(dir, name))]


PRINT_VERBOSE = False


def print_verbosely(*args):
    if PRINT_VERBOSE:
        print(*args)


def assert_report(cond, message, *filenames):
    if not cond:
        print('\n\n\n\n')
        print(message)
        for filename in filenames:
            f = open(filename)
            text = f.read()
            print('\n\n\n')
            print(filename + ':')
            print('\n')
            print(text)
            f.close()
        assert (0)


def assure_working_directories():
    if not os.path.isdir(DIR_INTERMEDIATE_FILES):
        os.makedirs(DIR_INTERMEDIATE_FILES)
    if not os.path.isdir(DIR_OUTPUT_FILES):
        os.makedirs(DIR_OUTPUT_FILES)


#################################################################################
### COMPARE/UPDATE EXPECTED RESULTS
#################################################################################

def hide_absolute_path(filename):
    hide_command = "sed -i -e 's/\/.*\//\/abs_path\//g' {0}\n".format(filename)
    return_code = call(hide_command, shell=True)
    return return_code


def hideAddresses(filename):
    hide_command = "sed -i -e 's/\[0x.*,+/[0xdeadbeaf,+/g' {0}".format(filename)
    return_code = call(hide_command, shell=True)
    return return_code


# test name is the filename (WITHOUT EXTENSION)
# -> then compare filename.exp with filename.out and generate filename.err
def is_one_output_correct(directory, test_name):
    filename1 = test_name + '.exp'
    assert (os.path.isfile(filename1)), "cannot find file " + filename1
    filename2 = test_name + '.out'
    assert (os.path.isfile(filename2)), "cannot find file " + filename2
    filename_diff = test_name + '.diff'
    diff_f = open(filename_diff, 'w')

    output_correct = True
    lines_exp = open(filename1).readlines()
    lines_got = open(filename2).readlines()
    for line in difflib.context_diff(lines_exp, lines_got, fromfile='before.py', tofile='after.py'):
        output_correct = False
        diff_f.write(line)

    # if the diff is empty do not leave the diff file
    diff_f.close()
    if (output_correct):
        os.remove(filename_diff)
        os.remove(filename2)
    else:
        print("error in test: " + directory)
        print("files {0} and {1} are different".format(filename1, filename2))
        print("check differences:")
        print("\tvim {0}/{1}".format(directory, filename_diff))
        print("\tmeld {0}/{1} {0}/{2}".format(directory, filename1, filename2))

    return output_correct


# take the list of prefixes and compare the output of each of them
def are_outputs_correct(directory, tested_files_prefixes):
    correct = True
    for test in tested_files_prefixes:
        correct = correct and is_one_output_correct(directory, test)
    return correct


# get the output (<test_name>.out) and copy it to be the expected result (<test_name>.exp)
# so update what is expected
def update_one_test_expected_result(test_name):
    filename_output = test_name + '.out'
    assert (os.path.isfile(filename_output)), "cannot find file " + filename_output
    filename_expected = test_name + '.exp'
    shutil.copy2(filename_output, filename_expected)


# take the list of prefixes and update each of them
def update_expected_results(tested_files_prefixes):
    for test in tested_files_prefixes:
        update_one_test_expected_result(test)


def compare_update_expected(directory,
                            do_regenerate_expected_output,
                            still_successful,
                            *tested_files_prefixes):
    if do_regenerate_expected_output:
        # just make .out files to be correct -> copy them to .exp files
        update_expected_results(tested_files_prefixes)
    else:
        # test if .out files are equal to .exp files
        still_successful = still_successful and are_outputs_correct(directory, tested_files_prefixes)
    return still_successful


#################################################################################
### CLO
#################################################################################

def parse_command_line():
    # parsing command line
    usage = "usage: %prog test_name [options]"
    parser = OptionParser(usage=usage + ' test_name=all -> do all tests')

    parser.add_option("-r", action="store_true", dest="do_regenerate_expected_output",
                      help="copy .out tests to .exp tests for the selected tests", default=False)
    parser.add_option("-x", action="store_true", dest="do_delete_intermediate_files",
                      help="delete all precomputed files -> next tests do all computation from scratch", default=False)
    parser.add_option("-c", action="store_true", dest="do_compile",
                      help="compile the tests", default=False)
    parser.add_option("-t", action="store_true", dest="do_tracing",
                      help="execute the tests", default=False)
    parser.add_option("-i", action="store_true", dest="do_instrument",
                      help="do translation of log into fast_deep_log", default=False)
    parser.add_option("-s", action="store_true", dest="do_select_decomposition",
                      help="do translation with selected decomposition", default=False)
    parser.add_option("-d", action="store_true", dest="do_driver_testing",
                      help="do driver tests", default=False)
    parser.add_option("-b", action="store_true", dest="do_bottlenecks_testing",
                      help="do bottleneck_tasks tests", default=False)
    parser.add_option("-v", action="store_true", dest="print_verbose",
                      help="print verbosely the output of tests", default=False)
    parser.add_option("--all", action="store_true", dest="do_all_tests",
                      help="do all possible tests", default=False)
    parser.add_option("--instrumentation", dest="instrumentation_tool",
                      help="which tool to use for instrumentation", default='llvm')

    (options, args) = parser.parse_args()

    return (options, args)


#################################################################################
### POTENTIAL REQUESTED OPERATIONS
#################################################################################

def delete_intermediate_files(directory):
    intermediate_folder = directory + '/' + DIR_INTERMEDIATE_FILES
    if os.path.isdir(intermediate_folder):
        shutil.rmtree(intermediate_folder)



#### compilation ###
def compile_test(directory, do_regenerate_expected_output, test_successful_so_far):
    assure_working_directories()
    #util_fncs.panic('about to compile with mcc')
    print('inside compile_test_mcc')
    compile_command = "make"
    stdout_filename = "compilation.out"
    stderr_filename = "compilation.err"
    compile_command += " >{0} 2>{1}".format(stdout_filename, stderr_filename)
    print_verbosely(compile_command)
    return_code = call(compile_command, shell=True)
    print_verbosely(return_code)
    assert_report(return_code == 0,
                  "failed compilation in test {0}".format(directory), 
                  stdout_filename, stderr_filename)
    still_successful = compare_update_expected(directory,
                                               do_regenerate_expected_output,
                                               test_successful_so_far,
                                               "compilation")
    return still_successful

def assure_compiled(directory):
    if not os.path.isfile('all.exe'):
        print("to recompile test: " + directory)
        compile_test(directory)




#### execution ###
def ompss_tracing(directory, do_regenerate_expected_output, test_successful_so_far):
    execute_command = "./exe.sh"
    stdout_filename = "execution.out"
    stderr_filename = "execution.err"
    execute_command += " >{0} 2>{1}".format(stdout_filename, stderr_filename)
    #print_verbosely(execute_command)
    return_code = call(execute_command, shell=True)
    print_verbosely(return_code)
    assert_report(return_code == 0,
                  "failed execution in test {0}".format(directory),
                  stdout_filename, stderr_filename)
    still_successful = compare_update_expected(directory,
                                               do_regenerate_expected_output,
                                               test_successful_so_far,
                                               "execution")
    return still_successful











#################################################################################
### MAINISH
#################################################################################

def run_all_tests(options, args):
    if options.do_all_tests:
        do_compile = True
        do_tracing = True
        do_instrument = True
    else:
        do_compile = options.do_compile
        do_tracing = options.do_tracing
        do_instrument = options.do_instrument

    do_regenerate_expected_output = options.do_regenerate_expected_output
    do_delete_intermediate_files = options.do_delete_intermediate_files
    PRINT_VERBOSE = options.print_verbose
    print (PRINT_VERBOSE)


    #return_code = call("source environment.bash", shell=True)
    #assert (return_code ==0)
  
    if (len(args) == 0):
        tests = 'all'
    else:
        tests = args


    if (tests == 'all'):
        directories = get_immediate_subdirectories('.')
        print_verbosely (directories)
    else:
        directories = list(map(lambda x: x.rstrip('/'), tests))
        #print (directories)
        for test_dir in directories:
            assert (test_dir in get_immediate_subdirectories('.')), "test {0} not in the suite".format(test_dir)

    #util_fncs.assure_binaries_in_paths(BINARIES)

    # PRINT THE HEADER FOR TESTS
    print('#################################################################')
    print("TO RUN TESTS:")
    print("do_delete_intermediate_files:    {0}".format(do_delete_intermediate_files))
    print("do_compile:                      {0}".format(do_compile))
    print("do_instrument:                   {0}".format(do_instrument))
    print("do_regenerate_expected_output:   {0}".format(do_regenerate_expected_output))
    total_tests = len(directories)
    print("TOTAL TESTS:                     {0}".format(total_tests))
    print('#################################################################\n')

    # if I need to delete precomputed intermediate files (binary, instrumentation log ...)
    if do_delete_intermediate_files:
        for directory in directories:
            delete_intermediate_files(directory)

    successful_tests = 0
    # GO TO EVERY DIRECTORY AND EXECUTE RUN TEST FOR EACH DIRECTORY
    for directory in directories:

        test_successful_so_far = True
        print("testing: " + directory)
        os.chdir(directory)

        # compiling
        if do_compile:
            test_successful_so_far = compile_test(directory, do_regenerate_expected_output, test_successful_so_far)

	# tracing
        if do_tracing:
            test_successful_so_far = ompss_tracing(directory, do_regenerate_expected_output, test_successful_so_far)
	

        #was it correct or not
        if test_successful_so_far:
            successful_tests += 1
        else:
            print('     !!!failed test: ' + directory)

        # exit from this test
        os.chdir('..')

    print('\n#################################################################')
    print("SUCCESSFUL TESTS:   {0}/{1}".format(successful_tests, total_tests))
    print('#################################################################')




if __name__ == '__main__':
    #print ("parsed cmd")
  
    (options, args) = parse_command_line()
    #print ("parsed cmd")
    run_all_tests(options, args)


