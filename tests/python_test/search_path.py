from os.path import isdir

def search_valid_pass(potential_paths):

    for path in potential_paths:
        if isdir(path):
            return path

    raise Exception("You must run the testsuite in a appropriate directory\n")

def get_path_test():

    potential_paths = ["python_test/test_in"]
    return search_valid_pass(potential_paths)