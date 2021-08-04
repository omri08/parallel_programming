#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
enum ranks{ROOT,M=200,N=105};


void generateNumbers(int* arr, int size)
{
	int i;

	srand(time(NULL));
	for (i = 0; i < size; i++)
	{
		arr[i] = rand() % 10000000;
        
	}
}

int main(int argc, char *argv[])
{

    int my_rank, num_procs,arrsize, my_max, Max,i;
    int *arr, *work_arr;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); 

    arrsize = M*N*num_procs;

    if(my_rank == ROOT)
    {
        arr = (int*)malloc(sizeof(int)*arrsize);
        generateNumbers(arr,arrsize);
    }

    work_arr =  (int*)malloc(sizeof(int)*arrsize/num_procs);
    MPI_Scatter(arr,arrsize/num_procs,MPI_INT,work_arr,arrsize/num_procs,MPI_INT,ROOT,MPI_COMM_WORLD);
    my_max = work_arr[0];
    for(i=0;i<arrsize/num_procs;i++)
    {
        if(work_arr[i] > my_max)
            my_max = work_arr[i];
    }
    MPI_Reduce(&my_max,&Max,1,MPI_INT,MPI_MAX,ROOT,MPI_COMM_WORLD);

    if(my_rank == ROOT)
    {
        printf("Parallel Max %d\n",Max);
        my_max = arr[0];
        for(i=0;i<arrsize;i++)
        {
            if(arr[i] > my_max)
                my_max = arr[i];
        }
        printf("Sequential Max %d\n",my_max);
        free(arr);
    }
    free(work_arr);
   

    MPI_Finalize();
    return 0;
}