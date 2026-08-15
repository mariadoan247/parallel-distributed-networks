#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
    // Catch console errors
    if (argc != 3)
    {
        printf("USE LIKE THIS: pingpong_MPI n_items time_prob1_MPI.csv\n");
        return EXIT_FAILURE;
    }

    /* Read in command line items */
    int n_items = strtol(argv[1], NULL, 10);
    FILE* outputFile = fopen(argv[2], "w");
    int* ping_array = (int*)malloc(n_items * sizeof(int));

    /* Start up MPI */
    int my_rank;    /* My process rank */
    MPI_Init(NULL, NULL);
    
    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Start time
    double starttime;
    starttime = MPI_Wtime();




    // Create your MPI program.
    if (my_rank == 0) {
    
        // Fill array with incremental values
        for (int i = 0; i < n_items; i++)
            ping_array[i] = i;

        // if myrank is 0
        for (int i = 0; i < 1000; ++i) {
            MPI_Send(ping_array, n_items, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(ping_array, n_items, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // End time
        double endtime = MPI_Wtime();
        // output
        fprintf(outputFile, "%f", endtime-starttime);
        free(ping_array);
    }
    else {

        // if my rank not 0
        for (int i = 0; i < 1000; ++i) {
            MPI_Recv(ping_array, n_items, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(ping_array, n_items, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
} /* main */

