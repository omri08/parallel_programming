build:
	mpicc -c main.c 
	mpicc -o exec main.o

clean:
	rm *.o exec

run2:
	mpiexec -np 2 exec 