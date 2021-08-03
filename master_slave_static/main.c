#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

enum ranks{ROOT,M=8000,N=800};

int generateNumbers(int* arr, int size);

int findMax(int* arr, int size);

int main(int argc, char *argv[])
{

    int my_rank, num_procs,*arr,my_arr[N],arr_size,local_max,global_max=0,jobs_done=0,temp_max;
    double start;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    arr_size=num_procs*M*N;

    if(my_rank == ROOT)
    {
        arr = (int*)malloc(arr_size*sizeof(int));
        generateNumbers(arr,arr_size);
        start = MPI_Wtime();
    }

    while(jobs_done <arr_size)
    {
        MPI_Scatter(arr+jobs_done,N,MPI_INT,my_arr,N,MPI_INT,ROOT,MPI_COMM_WORLD);

        local_max = findMax(my_arr,N);

        MPI_Reduce(&local_max,&temp_max,1,MPI_INT,MPI_MAX,ROOT,MPI_COMM_WORLD);

        if(my_rank == ROOT)
        {
            if(temp_max > global_max)
                global_max = temp_max;
        }
        jobs_done+= N*num_procs;
    }
    if(my_rank == ROOT)
    {
        printf("Parallel: The global max is :%d\n",global_max);
        printf("Time %lf\n",MPI_Wtime()-start);
        //control
        start = MPI_Wtime();
        global_max = findMax(arr,arr_size);
        printf("Serial: The global max is :%d\n",global_max);
        printf("Time %lf\n",MPI_Wtime()-start);
    }


    
    
    MPI_Finalize();
    return 0;
}



int generateNumbers(int* arr, int size)
{
	int i;

	srand(time(NULL));
	for (i = 0; i < size; i++)
	{
		arr[i] = rand() % 100000001;
	}
}

int findMax(int* arr, int size)
{
	int i, max = -1;

	for (i = 0; i < size; i++)
	{
		if(arr[i]>max)
            max =  arr[i];
	}

	return max;
}