#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <iostream>

using namespace std;

int main() {
    int rank, size;
    int N;

    MPI_Init(NULL, NULL);  // Initialize MPI environment
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get the rank of the current process

    if (rank == 0) {
        cout << "Enter the size of the matrix (N): ";
        cin >> N;
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);  // Broadcast N to all processes

    int* A = new int[N * N];         // Matrix A
    int* B = new int[N];             // Vector B
    int* localresult = new int[N]();  // Local result for each process
    int* result = new int[N]();       // Final result

    for (int i = 0; i < N; i++) {
        localresult[i] = 0;
        result[i] = 0;
    }

    if (rank == 0) {
        srand(time(NULL));  // Seed the random number generator

        // Initialize matrix A with random values
        for (int i = 0; i < N * N; i++)
            A[i] = rand() % 3; // Assuming values are between 0 and 9

        // Initialize vector B with random values
        for (int i = 0; i < N; i++)
            B[i] = rand() % 3; // Assuming values are between 0 and 9

        // Print matrix A
        cout << "Matrix A:" << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << A[i * N + j] << " ";
            }
            cout << endl;
        }

        // Print vector B
        cout << "Vector B:" << endl;
        for (int i = 0; i < N; i++) {
            cout << B[i] << " ";
        }
        cout << endl;
    }

    int part;
    // Calculate the number of columns per process
    if (N < size) {
        part = 1;
    }
    else {
        part = N / size;
    }
    int remainder = N % size;

    // Allocate memory for sub-matrix A and sub-vector B
    int* subA = new int[N * part];
    int* subB = new int[part];

    // Scatter matrix A to all processes
    MPI_Scatter(A, N * part, MPI_INT, subA, N * part, MPI_INT, 0, MPI_COMM_WORLD);
    // Scatter vector B to all processes
    MPI_Scatter(B, part, MPI_INT, subB, part, MPI_INT, 0, MPI_COMM_WORLD);

    if (N > rank) {
        // Perform matrix-vector multiplication locally
        for (int j = 0; j < part; j++) {
            for (int i = 0; i < N; i++) {
                localresult[i % N] += subB[j] * subA[i + N * j];
                // Print localresult and subB * subA for each process
            }
        }
    }

    // Scatter the remainder of matrix A and vector B
    if (rank < remainder && N > size) {
        // Calculate the start index for the remainder part of A and B
        int start_index = part * size + rank;
        MPI_Scatter(&A[start_index * N], N, MPI_INT, subA, N, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&B[start_index], 1, MPI_INT, subB, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // Perform matrix-vector multiplication locally for the remainder part
        for (int i = 0; i < N; i++) {
            localresult[i] += subB[0] * subA[i];
        }
    }

    // Reduce local results to get the final result
    MPI_Reduce(localresult, result, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nResult\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", result[i]);
        }
    }

    delete[] A;
    delete[] B;
    delete[] localresult;
    delete[] result;
    delete[] subA;
    delete[] subB;

    MPI_Finalize(); // Finalize MPI environment

    return 0;
}