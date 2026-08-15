#include <stdio.h>
#include <stdlib.h> 
// Use more libraries as necessary

#define DEBUG 0

/* ---------- Project 1 - Problem 2 - Mat-Vec Mult ----------
    This file will multiply a matrix and vector.
    Complete the TODOs left in this file.
    Sources:
        https://stackoverflow.com/questions/2128728/allocate-matrix-in-c

*/ // ------------------------------------------------------ //


int main (int argc, char *argv[])
{
    // Catch console errors
    if( argc != 7)
    {
        printf("USE LIKE THIS: serial_mult_mat_vec in_mat.csv n_row_1 n_col_1 in_vec.csv n_row_2 output_file.csv \n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE *matFile = fopen(argv[1], "r");
    FILE *vecFile = fopen(argv[4], "r");

    // Get dim of the matrix
    char* p1;
    char* p2;
    int n_row1 = strtol(argv[2], &p1, 10 );
    int n_col1 = strtol(argv[3], &p2, 10 );

    // Get dim of the vector
    char* p3;
    int n_row2 = strtol(argv[5], &p3, 10 );

    // Get the output file
    FILE *outputFile = fopen(argv[6], "w");


    // Use malloc to allocate memory for the matrices
    // Using long int to prevent number from running out of range

    // Storing matrix in a dynamically allocated 1D array in the row-major order
    long int *mat = (long int *)malloc(n_row1 * n_col1 * sizeof(long int));
    long int *vec_in = (long int*)malloc(n_row2 * sizeof(long int));
    // The output vector will have the same number as rows as the input matrix
    // according to how matrix vector multiplication works
    long int *vec_out = (long int*)malloc(n_row1 * sizeof(long int));


    // Parse the input CSV files

    long int temp = 0;  // Temp variable for parsed values
    long int offset = 0;     // Offset for matrix indexing

    // Parse the matrix
    for (int i = 0; i < n_row1; i++)        // Iterate matrix rows
    {
        for (int j = 0; j < n_col1; j++)    // Iterate matrix columns
        {
            fscanf(matFile, "%ld,", &temp); // Store int value into temp
            offset = i * n_col1 + j;        // Set Offset for row-major ordering
            mat[offset] = temp;             // Store parsed value in matrix
        }
    }

    // Parse the input vector
    for (int i = 0; i < n_row2; i++)        // Iterate input vector rows
    {
        fscanf(vecFile, "%ld\n", &temp);    // Store int value into temp
        vec_in[i] = temp;                   // Store parsed value in input vector
    }


    // Perform the matrix-vector multiplication

    for (int i = 0; i < n_row1; i++)        // Iterate matrix rows
    {
        for (int j = 0; j < n_col1; j++)    // Iterate matrix columns
        {
            offset = i * n_col1 + j;        // Set offset
            vec_out[i] += mat[offset] * vec_in[j]; 
        }
    }


    // Write the output CSV file

    for (int i = 0; i < n_row1; i++)    // Iterate output vector rows
    {
        fprintf(outputFile, "%ld\n", vec_out[i]);
    }


    //Free memory

    // Cleanup
    fclose (matFile);
    fclose (vecFile);
    fclose (outputFile);

    // Free buffers here!
    free(mat);
    free(vec_in);
    free(vec_out);

    return 0;
}
