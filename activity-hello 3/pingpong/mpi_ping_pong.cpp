#include <mpi.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

int main (int argc, char* argv[]) {
    MPI_Init (&argc, &argv);

    if (argc < 2) {
        std::cerr<<"usage: mpirun "<<argv[0]<<" <value>"<<std::endl;
        return -1;
    }

    std::chrono::time_point<std::chrono::system_clock> time_start = std::chrono::system_clock::now();

    int value = atoi(argv[1]);
    int size;
    int rank;
    int pid = getpid();
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    if (rank == 0) {
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
//        printf("pid 1: %d", pid);
    }
    else if (rank == 1) {
//        MPI_Status s;
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += 2;
        MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//        printf("pid 2: %d", pid);
    }

    if (rank == 0) {
        MPI_Recv(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << value << std::endl;
//        printf("pid 3: %d", pid);
    }
//    else if (rank == 1) {
//        MPI_Status s;
//        MPI_Recv(&value, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &s);
//    }

    std::chrono::time_point<std::chrono::system_clock> time_end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = time_end - time_start;

//    std::cout << value << std::endl;
//    std::cerr << elpased_seconds.count() << std::endl;

    MPI_Finalize();

    return 0;
}
