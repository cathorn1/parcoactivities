#include <mpi.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

int main(int argc, char*argv[]) {
    MPI_Init (&argc, &argv);

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout << "I am proccess " << rank << " out of " << size
    << ". I am running on machine: " << getpid() << std::endl;



    MPI_Finalize();

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = end-start;

    // display time to cerr
    std::cerr<<elpased_seconds.count()<<std::endl;
}
