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
    compile_command = "make compile"
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

#### selecting decomposition ###
def select_decomposition(directory, do_regenerate_expected_output, test_successful_so_far, task_list):
    assure_working_directories()
    assure_logs_translated(directory)
    assert_report(os.path.isfile(task_list),
                  "ERROR: did not provide the tasks_list.txt in directory {0}".format(directory))
    test_suffix = task_list.split('./' + TASKS_LIST_PREFIX)[1].split('.txt')[0]
    still_successful = test_successful_so_far
    objects_selected_types_new_prefix = OBJECTS_SELECTED_TYPES_PREFIX + test_suffix
    objects_selected_instances_new_prefix = OBJECTS_SELECTED_INSTANCES_PREFIX + test_suffix
    program_structure_new_prefix = PROGRAM_STRUCTURE_SELECTED_PREFIX + test_suffix
    tasks_selected_new_prefix = TASKS_SELECTED_PREFIX + test_suffix
    cum_obj_usage_new_prefix = CUM_OBJ_USAGE_PREFIX + test_suffix
    obj_usage_tasklist_new_prefix = OBJ_USAGE_TASKLIST_PREFIX + test_suffix
    obj_usage_deplist_new_prefix = OBJ_USAGE_DEPLIST_PREFIX + test_suffix
    dependencies_new_prefix = DEPENDENCIES_PREFIX + test_suffix
    dimemas_trace_new_prefix = DIMEMAS_TRACE_PREFIX + test_suffix
    pcf_addition_new_prefix = PCF_ADDITION_PREFIX + test_suffix
    xdot_graph_new_prefix = XDOT_GRAPH_PREFIX + test_suffix
    realized_tasks_new_prefix = REALIZED_TASKS_PREFIX + test_suffix
    objects_offsets_new_prefix = OBJECTS_OFFSETS_PREFIX + test_suffix
    objects_fragmentation_new_prefix = OBJECTS_FRAGMENTATION_PREFIX + test_suffix

    stdout_selection = DIR_INTERMEDIATE_FILES + '/selection.out'
    stderr_selection = DIR_INTERMEDIATE_FILES + '/selection.err'

    # binary
    command = "{0} ".format(BINARIES['TAREADOR_SELECT_DECOMP'])
    # deep logs
    command += " {0} {1} ".format(DEEP_LOG_EXECUTION_PREFIX + '.exp', DEEP_LOG_OBJECTS_PREFIX + '.exp')
    # decomposition configuration: tasks
    command += " {0} ".format(task_list)
    # log of the selected object types
    command += " --selected-object-types-log {0} ".format(objects_selected_types_new_prefix + '.out')
    # log of the selected object instances
    command += " --selected-object-instances-log {0} ".format(objects_selected_instances_new_prefix + '.out')
    # log of the program structure
    command += " --program-structure-log {0} ".format(program_structure_new_prefix + '.out')
    # log of the selected tasks (memory usage of tasks)
    command += " --tasks-usage-log {0} ".format(tasks_selected_new_prefix + '.out')
    # log of the cumulative object usage at the end of execution
    command += " --cumulative-objects-usage-log {0} ".format(cum_obj_usage_new_prefix + '.out')
    # log of the objects usage by tasks
    command += " --objects-usage-tasklist-log {0} ".format(obj_usage_tasklist_new_prefix + '.out')
    # log of the objects usage by dependencies
    command += " --objects-usage-deplist-log {0} ".format(obj_usage_deplist_new_prefix + '.out')
    # log of the identified dependencies
    command += " --dependencies-log {0} ".format(dependencies_new_prefix + '.out')
    # the trace to be simulated by Dimemas
    command += " --dimemas-trace {0} ".format(dimemas_trace_new_prefix + '.out')
    # the pcf file for Paraver with the names of the tasks
    command += " --pcf-tasknames {0} ".format(pcf_addition_new_prefix + '.out')
    # graph of task dependencies in dot format
    command += " --dependency-graph {0} ".format(xdot_graph_new_prefix + '.out')
    # list of the tasks realized in this decomposition (with extra info for the driver)
    command += " --realized-tasks {0} ".format(realized_tasks_new_prefix + '.out')
    # file with offsets for fast accessing logs
    command += " --offsets-objects-log {0} ".format(objects_offsets_new_prefix + '.out')
    # file with objects fragmentation information
    command += " --objects-fragmentation-log {0} ".format(objects_fragmentation_new_prefix + '.out')
    # collecting stdout and stderr
    command += " >{0} 2>{1} ".format(stdout_selection, stderr_selection)

    return_code = call(command, shell=True)
    assert_report((return_code == 0), "failed select decomposition in test {0}".format(directory),
                  stdout_selection, stderr_selection)

    # check the correctness of the log
    still_successful = compare_update_expected(directory,
                                               do_regenerate_expected_output,
                                               test_successful_so_far,
                                               objects_selected_types_new_prefix,
                                               objects_selected_instances_new_prefix,
                                               program_structure_new_prefix,
                                               tasks_selected_new_prefix,
                                               cum_obj_usage_new_prefix,
                                               obj_usage_tasklist_new_prefix,
                                               obj_usage_deplist_new_prefix,
                                               dependencies_new_prefix,
                                               dimemas_trace_new_prefix,
                                               pcf_addition_new_prefix,
                                               xdot_graph_new_prefix,
                                               realized_tasks_new_prefix,
                                               objects_offsets_new_prefix,
                                               objects_fragmentation_new_prefix
    )
    return still_successful


#### execution ###
def ompss_tracing(directory, do_regenerate_expected_output, test_successful_so_far):
    execute_command = "make execute"
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

#### driver running ###
def run_driver(directory, do_regenerate_expected_output, test_successful_so_far, driver_test):
    assert_report(os.path.isfile(driver_test),
                  "ERROR: did not provide the driver_test in directory {0}".format(directory))
    test_suffix = driver_test.split('./' + DRIVER_INPUT_TESTS_PREFIX)[1].split('.input')[0]

    # get the parameters of the test
    f = open(driver_test)
    for line in f.readlines():
        if line.startswith('driver_script'):
            driver_script = line.split(':')[1].strip()
        elif line.startswith('suffix'):
            suffix = line.split(':')[1].strip()
        elif line.startswith('args'):
            args = line.split(':')[1].strip()
        elif line.startswith('options'):
            options = line.split(':')[1].strip()
        elif line.startswith('test_correctness'):
            test_correctness = line.split(':')[1].strip().split(' ')
        elif line.startswith('redirected_output'):
            redirected_output = line.split(':')[1].strip()
        else:
            print('Error reading driver_test: {0}'.format(driver_test))
            print('error in line {0}'.format(line))
            assert (0)

    stdout_driver = DIR_INTERMEDIATE_FILES + '/driver.out'
    stderr_driver = DIR_INTERMEDIATE_FILES + '/driver.err'

    # COMPOSE THE COMMAND THAT EXECUTES THE REGTEST
    command = " {0} ".format(driver_script)
    # arguments
    command += " {0} ".format(args)
    # options
    command += " {0} ".format(options)
    # output log
    command += " >{0} 2>{1} ".format(stdout_driver, stderr_driver)

    print_verbosely(command)
    return_code = call(command, shell=True)
    assert_report((return_code == 0), "failed runing driver in test {0}:{1}".format(directory, driver_test),
                  stdout_driver, stderr_driver)

    if redirected_output != '':
        shutil.copy2(stdout_driver, redirected_output)

    # are the obtained results (files) the same as the expected
    tested_files_prefixes = [os.path.splitext(test)[0] for test in test_correctness]
    # check the correctness of the log
    still_successful = compare_update_expected(directory,
                                               do_regenerate_expected_output,
                                               test_successful_so_far,
                                               *tested_files_prefixes)
    return still_successful


# run all the driver tests in the directory
def driver_runs_multiple(directory, do_regenerate_expected_output, test_successful_so_far):
    driver_tests = glob.glob('./{0}*.input'.format(DRIVER_INPUT_TESTS_PREFIX))
    print('to execute {0} tests for driver_testing'.format(len(driver_tests)))

    still_successful = test_successful_so_far
    for driver_test in sorted(driver_tests):
        print_verbosely("driver test: ", driver_test)
        this_test_successful = run_driver(directory, do_regenerate_expected_output, test_successful_so_far, driver_test)
        still_successful = still_successful and this_test_successful

    return still_successful


#### bottlenecks testing -- running paramedir ###
def dimemas_simulation(directory, test, ncores, dimemas_trace_new_prefix, pcf_addition_new_prefix, folder_for_simulation):
    dimemas_trace_old_format = dimemas_trace_new_prefix + '.exp'
    pcf_addition = pcf_addition_new_prefix + '.exp'
    assert (os.path.isfile(dimemas_trace_old_format))
    assert (os.path.isfile(pcf_addition))

    stdout_trf2trf = DIR_INTERMEDIATE_FILES + '/trf2trf.out'
    stderr_trf2trf = DIR_INTERMEDIATE_FILES + '/trf2trf.err'
    # translate to new Dimemas format
    new_format_command = ' {0} '.format(BINARIES['TRF2TRF_BIN'])
    new_format_command += ' {0} '.format(dimemas_trace_old_format)
    new_format_command += ' {0} '.format(DIMEMAS_TRACE_NEW_FORMAT_TMP)
    new_format_command += " >{0} 2>{1} ".format(stdout_trf2trf, stderr_trf2trf)
    return_code = call(new_format_command, shell=True)
    assert_report(return_code == 0,
                  "failed runing trf2trf translation in bottleneck test {0}:{1}".format(directory, test),
                  stdout_trf2trf, stderr_trf2trf)

    stdout_simulation = DIR_INTERMEDIATE_FILES + '/simulation.out'
    stderr_simulation = DIR_INTERMEDIATE_FILES + '/simulation.err'
    # run Dimemas simulation
    dimemas_command = ' {0} '.format(BINARIES['DIMEMAS_SIMULATION_SCRIPT'])
    dimemas_command += ' {0} '.format(DIMEMAS_TRACE_NEW_FORMAT_TMP)
    dimemas_command += ' {0} '.format(ncores)
    dimemas_command += ' -t {0} '.format(pcf_addition)
    dimemas_command += ' -f {0} '.format(folder_for_simulation)
    dimemas_command += ' -u yes'
    # collecting stdout and stderr
    dimemas_command += " >{0} 2>{1} ".format(stdout_simulation, stderr_simulation)
    # print (dimemas_command)
    return_code = call(dimemas_command, shell=True)
    assert_report(return_code == 0,
                  "failed runing Dimemas simulation in bottleneck test {0}:{1}".format(directory, test),
                  stdout_simulation, stderr_simulation)

    paraver_trace = '{0}/prv_{1}cores.prv'.format(folder_for_simulation, ncores)
    # print (paraver_trace)
    assert (os.path.isfile(paraver_trace))
    return paraver_trace


# identify bottleneck tasks for the selected test, and confirm that the test passed correctly
def identify_bottleneck_tasks(directory, do_regenerate_expected_output, test_successful_so_far, test):
    print_verbosely("testing: " + test)
    assert_report(os.path.isfile(test),
                  "ERROR: did not provide the tasks_list.txt in directory {0}".format(directory))
    test_suffix = test.strip('./' + BOTTLENECKS_INPUT_TESTS_PREFIX).strip('.input')

    test_conf = eval(open(test).read())
    task_list = test_conf['tasks_list']
    conf_suffix = task_list.strip('./' + TASKS_LIST_PREFIX).strip('.txt')
    dimemas_trace_new_prefix = DIMEMAS_TRACE_PREFIX + conf_suffix
    pcf_addition_new_prefix = PCF_ADDITION_PREFIX + conf_suffix
    paraver_trace = dimemas_simulation(directory, test,
                                       test_conf['ncores'], \
                                       dimemas_trace_new_prefix, pcf_addition_new_prefix, \
                                       DIR_DIMEMAS_SIMULATIONS)
    tasks_costs_new_prefix = TASKS_COSTS_PREFIX + test_suffix

    # RUN THE BOTTLENECKS SCRIPT
    stdout_bottlenecks = DIR_INTERMEDIATE_FILES + '/bottlenecks.out'
    stderr_bottlenecks = DIR_INTERMEDIATE_FILES + '/bottlenecks.err'
    # binary
    command = "{0} ".format(BINARIES['TAREADOR_BOTTLENECK_TASKS'])
    # inputs
    command += " {0} {1} {2} ".format(pcf_addition_new_prefix + '.exp', \
                                      paraver_trace, \
                                      dimemas_trace_new_prefix + '.exp')
    # temp folder for storing paramedir files
    command += "--paramedir-output-folder {0} ".format(DIR_PARAMEDIR_PROCESSING)
    # outputs
    command += " {0} ".format(tasks_costs_new_prefix + '.out')
    # options
    command += " --length-weight {0} ".format(test_conf['pl'])
    command += " --dependencies-weight {0} ".format(test_conf['pd'])
    command += " --concurrency-weight {0} ".format(test_conf['pc'])
    command += " >{0} 2>{1} ".format(stdout_bottlenecks, stderr_bottlenecks)
    # print (command)

    return_code = call(command, shell=True)
    assert_report(return_code == 0, "failed runing bottleck script in bottleneck test {0}:{1}".format(directory, test),
                  stdout_bottlenecks, stderr_bottlenecks)

    # check the correctness of the log
    is_test_successful = compare_update_expected(directory,
                                                 do_regenerate_expected_output,
                                                 test_successful_so_far,
                                                 tasks_costs_new_prefix)
    # cleanup the files made when simulating and running paramedir
    shutil.rmtree(DIR_DIMEMAS_SIMULATIONS)
    shutil.rmtree(DIR_PARAMEDIR_PROCESSING)

    return is_test_successful


# read all bottleneck tests in this directory and run each of them
def bottlenecks_runs_multiple(directory, do_regenerate_expected_output, test_successful_so_far):
    bottleneck_tests = glob.glob('./{0}*.input'.format(BOTTLENECKS_INPUT_TESTS_PREFIX))
    print('to execute {0} tests for bottlenecks_testing'.format(len(bottleneck_tests)))

    still_successful = test_successful_so_far
    for test in sorted(bottleneck_tests):
        print_verbosely("bottleneck test: ", test)
        this_test_successful = identify_bottleneck_tasks(directory, do_regenerate_expected_output, still_successful,
                                                         test)
        still_successful = still_successful and this_test_successful

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


