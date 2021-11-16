#include <mpi.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char*argv[]) {
    MPI_Init (&argc, &argv);

    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout << "I am proccess " << rank << "out of " << size
    << ". I am running on machine: " << getpid() << std::endl;

    MPI_Finalize();
}
