#!/bin/bash
#SBATCH --partition=oucspdn_cpu
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --ntasks=1
#
#SBATCH --time=00:15:00
#
# --------------------- Change the items below here! ---------------------
#
#SBATCH --mail-user=mariadoan247@ou.edu
#SBATCH --mail-type=ALL
#
#SBATCH --job-name=hw5prob2
#
#SBATCH --chdir=/home/oucspdn086/Doan_Maria_Project_5/Problem_2/serial
#SBATCH --output=/home/oucspdn086/Doan_Maria_Project_5/Problem_2/serial/_P5-2_%J_stdout.txt
#SBATCH --error=/home/oucspdn086/Doan_Maria_Project_5/Problem_2/serial/_P5-2_%J_stderr.txt
#
#################################################

make clean
make

./dotprod_serial 262144 ../../../test_data/Vectors/vec1_2^18.csv ../../../test_data/Vectors/vec2_2^18.csv result_2^18.csv time_2^18.csv
# add other sizes
./dotprod_serial 524288 ../../../test_data/Vectors/vec1_2^19.csv ../../../test_data/Vectors/vec2_2^19.csv result_2^19.csv time_2^19.csv
./dotprod_serial 1048576 ../../../test_data/Vectors/vec1_2^20.csv ../../../test_data/Vectors/vec2_2^20.csv result_2^20.csv time_2^20.csv