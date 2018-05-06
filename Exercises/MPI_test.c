#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_LENGTH 10
//mpicc MPI_test.c -o MPI_test
//mpirun -np 4 ./MPI_test

main (int argc, char *argv[]){
    int i, tag = 0, tasks, iam, namelen, root=0;
    double x;
    char message[MSG_LENGTH+2], processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
    
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    if(iam == 0){
        strcpy(message, "Hola mundo ");
		MPI_Bcast(message, MSG_LENGTH, MPI_INT, root, MPI_COMM_WORLD);
    } else {

		MPI_Bcast(message, MSG_LENGTH, MPI_INT, root, MPI_COMM_WORLD);
		MPI_Comm_rank(MPI_COMM_WORLD, &iam);
		printf("node %d %s", iam, message);
		MPI_Get_processor_name(processor_name, &namelen);
		printf("processor %s\n", processor_name ); fflush(stdout);
	}
	MPI_Finalize();
}
