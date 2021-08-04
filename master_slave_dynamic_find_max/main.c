#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
enum ranks
{
    ROOT,
    M = 200,
    N = 105
};
enum tags
{
    WORK,
    STOP
};

void generateNumbers(int *arr, int size)
{
    int i;

    srand(time(NULL));
    for (i = 0; i < size; i++)
    {
        arr[i] = rand() % 10000000;
    }
}

int findMax(int *arr, int size)
{
    int i, max = arr[0];
    for (i = 1; i < size; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

void workerProcess()
{
    int arr[N], max, tag;
    MPI_Status status;
    do
    {
        MPI_Recv(arr, N, MPI_INT, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        tag = status.MPI_TAG;
        max = findMax(arr, N);
        MPI_Send(&max, 1, MPI_INT, ROOT, tag, MPI_COMM_WORLD);

    } while (tag != STOP);
}

void masterProcess(int num_procs)
{
    MPI_Status status;
    int *arr, arrsize, worker_id, jobs_sent = 0, tag;
    int localMax, globalMax;
    arrsize = num_procs * M * N;
    arr = (int *)malloc(sizeof(int) * arrsize);
    generateNumbers(arr, arrsize);
    globalMax = arr[0];
    double t = MPI_Wtime();
    //start workers
    for (worker_id = 1; worker_id < num_procs; worker_id++)
    {
        MPI_Send(arr + jobs_sent, N, MPI_INT, worker_id, WORK, MPI_COMM_WORLD);
        jobs_sent += N;
    }

    //recv and send more work
    printf("arrsize = %d\n", arrsize);
    for (; jobs_sent < arrsize; jobs_sent += N)
    {
        if (arrsize - jobs_sent <= (num_procs - 1) * N)
            tag = STOP;
        else
            tag = WORK;

        MPI_Recv(&localMax, 1, MPI_INT, MPI_ANY_SOURCE, WORK, MPI_COMM_WORLD, &status);
        MPI_Send(arr + jobs_sent, N, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
        if (localMax > globalMax)
            globalMax = localMax;
    }

    //terminate
    for (worker_id = 1; worker_id < num_procs; worker_id++)
    {
        MPI_Recv(&localMax, 1, MPI_INT, MPI_ANY_SOURCE, STOP, MPI_COMM_WORLD, &status);
        if (localMax > globalMax)
            globalMax = localMax;
    }
    printf("Parallel max is %d, time %lf\n", globalMax, MPI_Wtime() - t);
    t = MPI_Wtime();
    globalMax = findMax(arr, arrsize);
    printf("Sequential max is %d, time %lf\n", globalMax, MPI_Wtime() - t);
    free(arr);
}

int main(int argc, char *argv[])
{

    int my_rank, num_procs;
    int *arr;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (my_rank == ROOT)
    {
        masterProcess(num_procs);
    }
    else
    {
        workerProcess();
    }

    MPI_Finalize();
    return 0;
}