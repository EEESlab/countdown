// Mpi hello world program
//#include "mpe_log.h"
#include <stdio.h>
#include <mpi.h>

int main() {
    int rank, size;
    int event1, event2;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello world from rank %d of %d\n", rank, size);

    MPI_Finalize();
    return 0;
}
