#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int size, rank;
    char name[MPI_MAX_PROCESSOR_NAME];
    int len;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the name of the processor
        MPI_Get_processor_name(name, &len);

    // Print off a hello world message
    printf("Hello from processor %s, rank %d out of %d processors\n",
           name, rank, size);

    // Finalize the MPI environment.
    MPI_Finalize();
}