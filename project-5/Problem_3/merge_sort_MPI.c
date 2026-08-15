#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define MAXLINE 25
#define DEBUG   0

// to read in file
float* read_input(FILE* inputFile, int n_items);
int cmpfloat(const void* a, const void* b);

/* Main Program -------------- */
int main (int argc, char *argv[])
{
    if( argc != 5)
    {
        printf("USE LIKE THIS: merge_sort_MPI n_items input.csv output.csv time.csv\n");
        return EXIT_FAILURE;
    }

    // input file and size
    FILE* inputFile = fopen(argv[2], "r");
    int  n_items = strtol(argv[1], NULL, 10);

    // Start MPI
    int my_rank, comm_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    // arrays to use
    // initialize your arrays here
    int local_vec_size = n_items/comm_size;
    float* local_vec = malloc(n_items/comm_size * sizeof(float));
    float* global_vec;

    // get start time
    double local_start, local_finish, local_elapsed, elapsed;
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();



    // TODO: implement solution here
    // Step 1: Read a global array from an input file using process 0
    if (my_rank == 0) {
        global_vec = read_input(inputFile, n_items);
        fclose(inputFile);
    }

    // Step 2: Scatter the global array across all processes
    MPI_Scatter(global_vec, local_vec_size, MPI_FLOAT, local_vec, local_vec_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Step 3: Sort the local arrays by every process using the built-in quick sort function (qsort)
    qsort(local_vec, local_vec_size, sizeof(float), cmpfloat);

    // Step 4: Reduce the local sorted arrays on all processes to a global sorted array on process 0
    MPI_Gather(local_vec, local_vec_size, MPI_FLOAT, global_vec, local_vec_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // get elapsed time
    local_finish  = MPI_Wtime();
    local_elapsed = local_finish-local_start;

    // send time to main process
    MPI_Reduce(
        &local_elapsed, 
        &elapsed, 
        1, 
        MPI_DOUBLE,
        MPI_MAX, 
        0, 
        MPI_COMM_WORLD
    );

    // Write output (Step 5)
    if (my_rank == 0) {
        FILE* outputFile = fopen(argv[3], "w");
        FILE* timeFile = fopen(argv[4], "w");

        // output
        for (int i = 0; i < n_items; ++i) {
            fprintf(outputFile, "%f\n", global_vec[i]);
        }
        fprintf(timeFile, "%.20f", local_elapsed);

        fclose(outputFile);
        fclose(timeFile);
    }


    MPI_Finalize();

    if(DEBUG) printf("Finished!\n");
    return 0;
} // End Main //



/* Read Input -------------------- */
float* read_input(FILE* inputFile, int n_items) {
    float* arr = (float*)malloc(n_items * sizeof(float));
    char line[MAXLINE] = {0};
    int i = 0;
    // char* ptr;
    while (fgets(line, MAXLINE, inputFile)) {
        sscanf(line, "%f", &(arr[i]));
        ++i;
    }
    return arr;
} // Read Input //



/* Cmp Int ----------------------------- */
// use this for qsort
// source: https://stackoverflow.com/questions/3886446/problem-trying-to-use-the-c-qsort-function
int cmpfloat(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
} // Cmp Int //
