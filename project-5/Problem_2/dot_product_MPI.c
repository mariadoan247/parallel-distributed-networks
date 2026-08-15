#include <stdio.h>
#include <stdlib.h> // for strtol
#include <string.h>
#include <time.h>
#include "mpi.h"

#define MAXCHAR 25
#define BILLION  1000000000.0

int main (int argc, char *argv[])
{
    if( argc != 6)
    {
        printf("USE LIKE THIS: dot_product_MPI vector_size vec_1.csv vec_2.csv result.csv time.csv\n");
        return EXIT_FAILURE;
    }

    // Size
    int vec_size = strtol(argv[1], NULL, 10);

    // Input files
    FILE* inputFile1 = fopen(argv[2], "r");
    FILE* inputFile2 = fopen(argv[3], "r");
    if (inputFile1 == NULL) printf("Could not open file %s", argv[2]);
    if (inputFile2 == NULL) printf("Could not open file %s",argv[3]);

    // Output files
    FILE* outputFile = fopen(argv[4], "w");
    FILE* timeFile = fopen(argv[5], "w");

    /* Start up MPI */
    int comm_sz, my_rank;
    float dot_product = 0.0;
    MPI_Comm comm;
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    MPI_Bcast(&vec_size, 1, MPI_FLOAT, 0, comm);
    int local_vec_size = vec_size/comm_sz;
    float* local_vec_1 = malloc(local_vec_size * sizeof(float));
    float* local_vec_2 = malloc(local_vec_size * sizeof(float));

    // To read in
    float* vec_1 = malloc(vec_size * sizeof(float));
    float* vec_2 = malloc(vec_size * sizeof(float));

    if (my_rank == 0) {
        // Store values of vector
        int k = 0;
        char str[MAXCHAR];
        while (fgets(str, MAXCHAR, inputFile1) != NULL)
        {
            sscanf( str, "%f", &(vec_1[k]) );
            k++;
        }
        MPI_Scatter(vec_1, local_vec_size, MPI_FLOAT, local_vec_1, local_vec_size, MPI_FLOAT, 0, comm);
        fclose(inputFile1);
        free(vec_1);

        // Store values of vector
        k = 0;
        while (fgets(str, MAXCHAR, inputFile2) != NULL)
        {
            sscanf( str, "%f", &(vec_2[k]) );
            k++;
        }
        MPI_Scatter(vec_2, local_vec_size, MPI_FLOAT, local_vec_2, local_vec_size, MPI_FLOAT, 0, comm);
        fclose(inputFile2);
        free(vec_2);
    } else {
        MPI_Scatter(vec_1, local_vec_size, MPI_FLOAT, local_vec_1, local_vec_size, MPI_FLOAT, 0, comm);
        MPI_Scatter(vec_2, local_vec_size, MPI_FLOAT, local_vec_2, local_vec_size, MPI_FLOAT, 0, comm);
        free(vec_1);
        free(vec_2);
    }
  
    // Start time
    double starttime;
    starttime = MPI_Wtime();

    // Get dot product
    float local_dot_product = 0.0;
    for (int i = 0; i < local_vec_size; i++)
        local_dot_product += local_vec_1[i] * local_vec_2[i];

    if (my_rank == 0) {

        MPI_Reduce(&local_dot_product, &dot_product, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

        printf("DP in C: %f\n", dot_product);

        // End time
        double endtime = MPI_Wtime();

        // Print to output
        fprintf(outputFile, "%f", dot_product);
        fprintf(timeFile, "%.20f", endtime-starttime);

        // Cleanup
        fclose (outputFile);
        fclose (timeFile);
    } else {
        MPI_Reduce(&local_dot_product, &dot_product, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
}

