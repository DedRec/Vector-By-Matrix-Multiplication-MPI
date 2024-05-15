# Vector-By-Matrix-Multiplication-MPI

This project implements a parallel matrix-vector multiplication using MPI (Message Passing Interface). Each process computes a portion of the resulting vector by distributing matrix columns and vector elements across processes.

## Requirements
- C++ compiler
- MPI library

## How to Run
1. Compile the code:
    ```bash
    mpic++ main.cpp -o matrix_vector_mult
    ```

2. Execute the compiled program with the desired number of MPI processes:
    ```bash
    mpiexec -n <num_processes> ./matrix_vector_mult
    ```

## Description
The program initializes a dense matrix A and a vector B with random values. It then distributes matrix columns and vector elements across MPI processes. Each process performs local matrix-vector multiplication, computing a portion of the resulting vector. Finally, the local results are reduced to obtain the final result.

## File Structure
- `main.cpp`: Contains the MPI-based implementation of matrix-vector multiplication.
