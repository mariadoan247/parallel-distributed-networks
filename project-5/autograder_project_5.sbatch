#!/bin/bash
# Choose the partition that you will use.
# You can choose: oucspdn_cpu OR debug_5min OR debug OR others 
#
#SBATCH --partition=oucspdn_cpu
#
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=8
#
#SBATCH --time=00:15:00
#SBATCH --job-name=p5_test
#
#
# --------------------- Change the items below here! ---------------------
#
#SBATCH --mail-user=mariadoan247@ou.edu
#SBATCH --mail-type=ALL
#
#SBATCH --chdir=/home/oucspdn086/Doan_Maria_Project_5/
#SBATCH --output=/home/oucspdn086/Doan_Maria_Project_5/autograding_%J_stdout.txt
#SBATCH --error=/home/oucspdn086/Doan_Maria_Project_5/autograding_%J_stderr.txt

#################################################

module purge

module load SciPy-bundle/2020.03-foss-2020a-Python-3.8.2
# module load OpenMPI

module load intel/2022.2
module load OpenMPI/4.1.4-GCC-11.3.0

# To run all the problems and get all results
python3 -u autograder_project_5.py

# To run just problem 2's autograder
# python3 -u autograder_problem_5_2.py

# To run just problem 3's autograder
# python3 -u autograder_problem_5_3.py

# To run just problem 4's autograder
# python3 -u autograder_problem_5_4.py