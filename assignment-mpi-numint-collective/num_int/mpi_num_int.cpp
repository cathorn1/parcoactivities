#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

double integrateNum (int func, int points, double lower, double upper, int intensity, int rank, int size) {

    double itgr_output = 0.0;
    double x = 0.0;
    double integral = 0.0;

    int begin = rank*(points/size);
    int end = ((rank+1)*(points/size));
    if (rank + 1 == size){
        end += points%size;
    }

    for (int i = begin; i < end; i++) {

        if (func == 1) {
            //for (int i = 0; i <= (points - 1); i++) {
                x = ((lower + (i + .5)) * ((upper - lower) / points));
                itgr_output += ((upper - lower) / points) * f1(x, intensity);
           // }
        } else if (func == 2) {
            //for (int i = 0; i <= (points - 1); i++) {
                x = ((lower + (i + .5)) * ((upper - lower) / points));
                itgr_output += ((upper - lower) / points) * f2(x, intensity);
            //}
        } else if (func == 3) {
          //  for (int i = 0; i <= (points - 1); i++) {
                x = ((lower + (i + .5)) * ((upper - lower) / points));
                itgr_output += ((upper - lower) / points) * f3(x, intensity);
           // }
        } else if (func == 4) {
           // for (int i = 0; i <= (points - 1); i++) {
                x = ((lower + (i + .5)) * ((upper - lower) / points));
                itgr_output += ((upper - lower) / points) * f4(x, intensity);
           // }
        }
    }

//    if(rank != 0) {
//        //send integral to rank 0
//        MPI_Send(&itgr_output, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
//    }
//    else {
//            for (int i = 1; i < size; i++) {
//                double integp;
//                integp = MPI_Recv(&integral, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//                itgr_output += integp;
//            }
//
//    }

    return itgr_output;
}

int main (int argc, char* argv[]) {
  
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
    MPI_Init (&argc, &argv);

    int func = atoi(argv[1]);
    int lower = atoi(argv[2]);
    int upper = atoi(argv[3]);
    int points = atoi(argv[4]);
    int intensity = atoi(argv[5]);

    std::chrono::time_point<std::chrono::system_clock> time_start = std::chrono::system_clock::now();

    int size;
    int rank;
    int* data = new int[7];

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    data[0] = func;
    data[1] = lower;
    data[2] = upper;
    data[3] = points;
    data[4] = intensity;
    data[5] = size;
    data[6] = rank;
//    int chunk = points/size;

//    int begin = rank*(points/size);
//    int end = ((rank+1)*(points/size));
//    if (rank + 1 == size){
//        end += points%size;
//    }

    MPI_Bcast(data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double global_res;
    double local_res = integrateNum(func, points, lower, upper, intensity, rank, size);

    MPI_Reduce(&local_res, &global_res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    std::chrono::time_point<std::chrono::system_clock> time_end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = time_end - time_start;

    if (rank == 0) {
        std::cout << global_res << std::endl;
        std::cerr << elpased_seconds.count() << std::endl;
    }
    MPI_Finalize();


  return 0;
}
