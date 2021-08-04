#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
enum ranks
{
    ROOT
};

int isPrime(int num)
{

    for (int i = 2; i <= num - 1; i++)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int countPrimes(int *arr, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (isPrime(arr[i]))
            count++;
    }

    return count;
}

int main(int argc, char *argv[])
{
    int rank, num_procs, total = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int arrSize = 15, jobsDone = 0, temp_count = 0;
    int *arr = (int *)malloc(sizeof(int) * arrSize);

    if (rank == ROOT)
    {
        arr[0] = 3;
        arr[1] = 4;
        arr[2] = 8;
        arr[3] = 12;
        arr[4] = 5;
        arr[5] = 32;
        arr[6] = 7;
        arr[7] = 35;
        arr[8] = 13;
        arr[9] = 32;
        arr[10] = 7;
        arr[11] = 35;
        arr[12] = 13;
        arr[13] = 7;
        arr[14] = 35;
    }
    int *work_arr = (int *)malloc(sizeof(int) * arrSize);
    while (jobsDone < arrSize)
    {
        MPI_Scatter(arr + jobsDone, arrSize / num_procs, MPI_INT, work_arr, arrSize / num_procs, MPI_INT, ROOT, MPI_COMM_WORLD);
        int numOfPrimes = countPrimes(work_arr, arrSize / num_procs);
        MPI_Reduce(&numOfPrimes, &temp_count, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
        if (rank == ROOT)
        {
            total += temp_count;
        }
        jobsDone += (arrSize / num_procs) * num_procs;
    }
    if (rank == ROOT)
    {
        printf("num of primes is: %d\n", total);
    }
    MPI_Finalize();

    return 0;
}
