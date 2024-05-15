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

## Explanation of main.cpp

The `main.cpp` file contains the implementation of the parallel matrix-vector multiplication using MPI. Here's a breakdown of the logic implemented:

1. **Initialization**:
   - Initialize MPI environment.
   - Get the total number of processes and the rank of the current process.
   - Process 0 prompts the user to input the size of the matrix `N`.

2. **Broadcasting Size**:
   - Process 0 broadcasts the size `N` to all other processes using `MPI_Bcast`.

3. **Memory Allocation and Initialization**:
   - Memory is allocated for matrix `A`, vector `B`, local result, and final result.
   - Matrix `A` and vector `B` are initialized with random values on process 0.

4. **Data Distribution**:
   - Matrix `A` and vector `B` are distributed among processes using `MPI_Scatter`.
   - Each process receives a portion of matrix `A` and vector `B` to compute.

5. **Local Computation**:
   - Each process performs local matrix-vector multiplication.
   - For each element in the local portion of matrix `A`, it multiplies the corresponding element in vector `B` and accumulates the result in a local result array.

6. **Handling Remainder**:
   - If the number of processes is not a divisor of `N`, process 0 handles the remaining columns of matrix `A` and elements of vector `B` and distributes them to processes that require them.
   - The local computation is performed on these remaining elements as well.

7. **Result Aggregation**:
   - Local results are reduced using `MPI_Reduce` to obtain the final result on process 0.

8. **Output**:
   - Process 0 outputs the final result.

9. **Memory Deallocation and Finalization**:
   - Memory is deallocated, and the MPI environment is finalized.

