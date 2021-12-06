#include <mpi.h>
#include <iostream>

int main (int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr<<"usage: mpirun "<<argv[0]<<" <value>"<<std::endl;
        return -1;
    }

    std::chrono::time_point<std::chrono::system_clock> time_start = std::chrono::system_clock::now();



    std::chrono::time_point<std::chrono::system_clock> time_end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = time_end - time_start;

    std::cerr << elpased_seconds.count() << std::endl;

    return 0;
}
