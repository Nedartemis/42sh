from argparse import ArgumentParser
from pathlib import Path
from difflib import unified_diff
from TestCase import TestCase
from os.path import isfile, join, isdir, abspath
from os import listdir
import sys
from search_path import get_path_test

import subprocess
import termcolor
import yaml

OK_TAG = f"[{termcolor.colored('OK', 'green')}]"
KO_TAG = f"[{termcolor.colored('KO', 'red')}]"


def diff(expected: str, actual: str) -> str:
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return ''.join(unified_diff(expected_lines, actual_lines,
                   fromfile="expected", tofile="actual"))


def run_shell(*cmd_line) -> subprocess.CompletedProcess:

    try:
        sh_proc = subprocess.run(cmd_line, capture_output=True, text=True, timeout=0.1)
    except:
        sh_proc = None

    return sh_proc


def perform_checks(test: TestCase,
                   actual: subprocess.CompletedProcess):
    errors = ""

    if actual == None:
        return "Error Time out\n\n"

    # expect error but get no error
    if test.stderr != "" and actual.stderr == "":
        errors += f"\nSomething was expected on stderr :\n--> {test.stderr}\n"
    
    # expect no error but get error
    if test.stderr == "" and actual.stderr != "":
        errors += f"\nNothing was expected on stderr, get : {actual.stderr}"
    
    # error exit code
    if test.exitcode != actual.returncode:
        mess = f"Exited with {actual.returncode} expected {test.exitcode}"
        errors += f"\n{termcolor.colored(mess, 'white')}\n"
    
    # error stdout
    if test.stdout != actual.stdout:
        errors += f"\nstdout differ\n{diff(test.stdout, actual.stdout)}\n"

    return None if errors == "" else errors

def run_test(dir_name, file_data_yml):

    with open(file_data_yml, "r") as file:
        testsuite = [TestCase(**test) for test in yaml.safe_load(file)]

    nb_fail = 0
    nb_success = 0
    mess = ""
    
    for testcase in testsuite:
        file = testcase.file

        sh_proc = run_shell(binary_path, file)

        errors = perform_checks(testcase, sh_proc)
        if errors:
            nb_fail += 1
            mess += f'{KO_TAG} {file}\n{errors}'
        else:
            nb_success += 1
            mess += f'{OK_TAG} {file}\n'

    if nb_fail > 0 and (all or dir_name in name_dirs_to_log):
        print_header(f"Test in {dir_name}", 'yellow')
        print(mess)

    return (nb_success, nb_fail)

def print_header(title, color):
    header = "=============== {} ===============".format(title)
    delim = "=" * len(header)
    all_header = "\n" + delim + "\n" + header + "\n" + delim + "\n"
    print(termcolor.colored(all_header, color))

def test_rec(test_repo):

    results = []
    nf = ns = 0

    for name_rep in listdir(test_repo):

        rep = join(test_repo, name_rep)

        if isdir(rep): # call recursively test_rec

            new_results = test_rec(rep)
            for (_, nb_fail, nb_success) in new_results:
                nf += nb_fail
                ns += nb_success
            results += new_results
        
        elif isfile(rep) and name_rep == "data.yml":

            name_dir = test_repo.split("/")[-1]
            nb_success, nb_fail = run_test(name_dir, rep)
            results.append((name_dir, nb_fail, nb_success))
            nf += nb_fail
            ns += nb_success

    if nf == 0 and ns > 0:
        return [(test_repo.split("/")[-1], nf, ns)]

    return results

def my_format(mess):
    n = 3
    mess = str(mess)
    if len(mess) < n:
        mess = " " * (n - len(mess)) + mess
    return mess
    

def test_suite(test_repo):

    results = []
    results += test_rec(test_repo)
    fails = success = 0

    print_header("Resume", 'green')
    for (name_dir, nb_fail, nb_success) in results:
        fails += nb_fail
        success += nb_success
        
        if nb_fail == 0:
            print(f'{OK_TAG}', end='')
        else:
            print(f'{KO_TAG}', end='')
        print(" [ %s | %s ] %s" % 
               (termcolor.colored(my_format(nb_fail), 'red'),
                termcolor.colored(my_format(nb_success), 'green'),
                name_dir))

    print(f"\nTotal Failsss = {termcolor.colored(fails, 'red')}")
    print(f"Total Success = {termcolor.colored(success, 'green')}")

def main():
    global binary_path

    if len(sys.argv) == 1:
        print("Error :")
        print("Usage : test_suite 42sh (dir_name)*")
        return

    executable = sys.argv[1]
    if (not isfile(executable)):
        print("Usage test_suite 42sh (dir_name)*")
        return
    
    name_dirs_to_log = []
    if len(sys.argv) > 2:
        name_dirs_to_log = sys.argv[2:]

    all = (name_dirs_to_log == [])
    binary_path = abspath(executable)
    print(binary_path)

    test_suite(get_path_test())

if __name__ == "__main__":

    main()

    
