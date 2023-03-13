from dataclasses import asdict
from os import listdir, remove
from os.path import isfile, join, isdir
from TestCase import TestCase
import re
from search_path import get_path_test

import subprocess
import yaml
import termcolor

def run_shell(*cmd_line) -> subprocess.CompletedProcess:
    return subprocess.run(cmd_line, capture_output=True, text=True)


def create_testcase_from_file(file: str) -> TestCase:
    process = run_shell("bash", "--posix", file)
    return TestCase(file, process.stdout, process.stderr,
                    process.returncode)

def create_testcases(test_repo):
    global n

    test_dicts = []

    for name_rep in listdir(test_repo): # go through the directory

        rep = join(test_repo, name_rep) # get the relative path of the rep

        if isdir(rep): # call recursively the function

            create_testcases(rep)

        elif name_rep == "data.yml":

            remove(rep)

        elif isfile(rep) and re.match(r".*\.in", name_rep): # create test case and add to dict

            n += 1

            test_case = create_testcase_from_file(rep)
            test_dicts.append(asdict(test_case))
        
        
    
    if test_dicts == []:
        return

    # write data.yml
    with open(join(test_repo, "data.yml"), 'w') as outfile:
         yaml.dump(test_dicts, outfile)


def print_header(title, color):
    header = "=============== {} ===============".format(title)
    delim = "=" * len(header)
    all_header = "\n" + delim + "\n" + header + "\n" + delim + "\n"
    print(termcolor.colored(all_header, color))

if __name__ == "__main__":

    n = 0

    create_testcases(get_path_test())

    
    print_header(f"{n} test created", "green")