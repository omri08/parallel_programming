build:
	mpicc -c main.c 
	mpicc -o exec main.o 

clean:
	rm -f *.o exec

run:
	mpiexec -n 2 ./exec

runOn2:
	mpiexec -n 2 -machinefile mf --map-by node ./exec