import numpy as np
import pandas as pd
import os

"""
    ~~~~~~~~~~~~~~~~~ Project 1   ver 1b ~~~~~~~~~~~~~~~~~
    This file is used to autograde a student's submission.
    To run this, do the following:
        1) Pay attention to the file tree structure given in the project guidelines.
        2) Place this file IN the directory that contains your problem folders.
            2.1) I.e. Place this within 'ShawJes_HW/'
            2.2) Make sure "ShawJes_HW/" is replaced with your name and the correct format.
        3) Create a folder called "test_data" in the directory ABOVE this file, place all test files inside of it.
            3.1) You may rename this folder below.
            3.2) Please make sure that the test files are in the same subfolders as is on Schooner.
        4) Run this python file with either python or python3.
            4.1) On Schooner, I found that I would need to run 'python autograding_individual_1a.py'
            4.2) You might need to download packages in Schooner like numpy or pandas with 'module load numpy', etc.
            4.3) Off of Schooner, you may need to use 'pip install numpy', etc.
            4.4) If you don't know how to use Python, a good IDE to use is Pycharm! It will make a venv for you.
        5) Receive your grade in the console.
        
    >> Make sure you have the structure below!:
    test_dir/         [..] 
        test_data/  
            Problem_2/    
        ShawJes_HW/    [.]   
            autograding_individual_1b.py   <--- Place here.
            Problem_1/
            Problem_2/
            ...
"""


# file locations
#   Note: this python file is located in './'.
test_dir  = "../test_data"  # this contains the test data
this_dir  = "."             # this is ShawJes_HW/

# change this to your name if you want (does not affect grading)
student_name = "Maria Doan"


# ------------------------------------------
# this will autograde one project submission
def autograde(in_this_dir, in_test_dir, in_student_name):

    # for mass grading purposes, ignore if individually grading
    student_name = in_student_name
    this_dir = in_this_dir
    test_dir = in_test_dir

    # test files
    td = test_dir + "/Problem_2/"
    t_inp_mat = [td + "test1_input_mat.csv",
                 td + "test2_input_mat.csv",
                 td + "test3_input_mat.csv"]
    t_inp_vec = [td + "test1_input_vec.csv",
                 td + "test2_input_vec.csv",
                 td + "test3_input_vec.csv"]
    t_out = [td + "test1_output_vec.csv",
             td + "test2_output_vec.csv",
             td + "test3_output_vec.csv"]
    t_res = ["test1_results.csv",
             "test2_results.csv",
             "test3_results.csv"]


    # copy makefile if not there
    #   **Note that this will use special commands, so you may have trouble running this on Windows or MacOS!**
    #   Use Schooner to run this file if you have trouble. You can use WinSCP to easily move your files there.
    dir_make  = this_dir + "Makefile"
    dir_prob1 = this_dir + "/Problem_2/"
    if not os.path.isfile(dir_prob1 + "/Makefile"):
        command = "(cp {} {})".format(dir_make, dir_prob1)
        os.system(command)


    # run make
    # print(this_dir) # debug
    dir_prob1 = this_dir + "/Problem_2/"
    command = "(cd {} && make)".format(dir_prob1)
    os.system(command)


    # test cases to run
    # serial_mult_mat_vec file_1.csv n_row_1 n_col_1 file_2.csv n_row_2 outputfile.csv
    tests = [ [t_inp_mat[0],  3,    3, t_inp_vec[0],   3, t_res[0]],
              [t_inp_mat[1],  10,  10, t_inp_vec[1],  10, t_res[1]],
              [t_inp_mat[2], 100, 100, t_inp_vec[2], 100, t_res[2]]]


    # execute tests
    for i in range(len(tests)):
        dir_exe    = this_dir + "/Problem_2/serial_mult_mat_vec"
        dir_output = this_dir + "/Problem_2/"
        command = "({} {} {} {} {} {} {} )".format(
            dir_exe, tests[i][0], tests[i][1], tests[i][2], tests[i][3], tests[i][4], dir_output + tests[i][5])
        os.system(command)
        # print(command) # debug


    # create the grade data frame
    col_names = [1, 2, 3, "Total"]
    grade = pd.DataFrame(
        np.nan,
        index   = [student_name],
        columns = [i for i in col_names]
    )


    # calculate the student grade
    count_success = 0
    for i in range(len(tests)):
        dir_prob1 = "/Problem_2/"
        results_benchmark_test_temp = np.genfromtxt(t_out[i], delimiter=',')
        results_test_temp           = np.genfromtxt(this_dir + dir_prob1 + t_res[i], delimiter=',')

        if len(results_benchmark_test_temp) != len(results_test_temp):
            print("Student:", student_name, "Test", (i + 1), "Different size")
            grade.loc[student_name, i + 1] = 0
            continue

        # calculating the sum of relative error
        diff_temp = np.sum(
            np.absolute((results_benchmark_test_temp - results_test_temp) / results_benchmark_test_temp))

        # print(diff_temp) #debug
        if diff_temp != 0:
            # print("Test", (i+1), "Failed") # debug
            grade.loc[student_name, i + 1] = 0
        else:
            # print("Test", (i+1), "Succeeded") # debug
            grade.loc[student_name, i + 1] = 1
            count_success += 1

    grade.loc[student_name, "Total"] = count_success
    return grade


# --------------------------------------------------------
# This will run autograde_HW1() IF you run this file only!
# This is so it is possible to grade multiple submissions with another python file.
if __name__ == "__main__":
    print("Autograding for Project 1.")
    grade = autograde(this_dir, test_dir, student_name)
    print(grade)