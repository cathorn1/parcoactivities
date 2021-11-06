#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <mutex>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

std::mutex mut;

void merge(int arr[], int l, int mid, int r) {

#if DEBUG
    std::cout<<l<<" "<<mid<<" "<<r<<std::endl;
#endif

    // short circuits
    if (l == r) return;
    if (r-l == 1) {
        if (arr[l] > arr[r]) {
            int temp = arr[l];
            arr[l] = arr[r];
            arr[r] = temp;
        }
        return;
    }

    int i, j, k;
    int n = mid - l;

    // declare and init temp arrays
    int *temp = new int[n];
    for (i=0; i<n; ++i)
        temp[i] = arr[l+i];

    i = 0;    // temp left half
    j = mid;  // right half
    k = l;    // write to

    // merge
    while (i<n && j<=r) {
        if (temp[i] <= arr[j] ) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = arr[j++];
        }
    }

    // exhaust temp
    while (i<n) {
        arr[k++] = temp[i++];
    }

    // de-allocate structs used
    delete[] temp;

}

//void mergeSort(int arr[], int n)
//{
//
//    int curr_size;  // For current size of subarrays to be merged
//    // curr_size varies from 1 to n/2
//    int left_start; // For picking starting index of left subarray
//    // to be merged
//
//    // Merge subarrays in bottom up manner.  First merge subarrays of
//    // size 1 to create sorted subarrays of size 2, then merge subarrays
//    // of size 2 to create sorted subarrays of size 4, and so on.
//    for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
//    {
//        // Pick starting point of different subarrays of current size
//        for (left_start=0; left_start<n-1; left_start += 2*curr_size)
//        {
//            // Find ending point of left subarray. mid+1 is starting
//            // point of right
//            int mid = std::min(left_start + curr_size - 1, n-1);
//
//            int right_end = std::min(left_start + 2*curr_size - 1, n-1);
//
//            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
//            //std::lock_guard<std::mutex> lck (mut);
//            merge(std::ref(arr), left_start, (mid+1), right_end);
//        }
//    }
//}


int main (int argc, char* argv[]) {
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  OmpLoop omp;
  omp.setNbThread(nbthreads);

  int chunk = n/nbthreads;
  int chunkRemain = n%nbthreads;

  // get arr data
  int* arr = new int [n];
  generateMergeSortData (arr, n);

//    for (int i =0; i < n; i++) {
//        std::cout << arr[i] << " ";
//    }
//    std::cout << "\n";


    #if DEBUG
        for (int i=0; i<n; ++i)
            std::cout<<arr[i]<<" ";
    std::cout<<std::endl;
    #endif

    // begin timing
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    omp.parfor<std::vector<int>>(0, n, chunk,
            [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    //std::cout << "p1\n";
//                    C.push_back(arr[i]);
//                }
            },
            [&](int i, std::vector<int> &C) -> void {

                int curr_size;  // For current size of subarrays to be merged
                // curr_size varies from 1 to n/2
                int left_start; // For picking starting index of left subarray
                // to be merged

                // Merge subarrays in bottom up manner.  First merge subarrays of
                // size 1 to create sorted subarrays of size 2, then merge subarrays
                // of size 2 to create sorted subarrays of size 4, and so on.
                for (curr_size=1; curr_size<=chunk-1; curr_size = 2*curr_size)
                {
                    // Pick starting point of different subarrays of current size
                    for (left_start=0; left_start<chunk-1; left_start += 2*curr_size)
                    {
                        // Find ending point of left subarray. mid+1 is starting
                        // point of right
                        int mid = std::min(left_start + curr_size - 1, n-1);

                        int right_end = std::min(left_start + 2*curr_size - 1, n-1);

                        // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
                        //std::lock_guard<std::mutex> lck (mut);
                        merge(std::ref(arr), left_start, (mid+1), right_end);
                    }
                }
//

//                std::cout << "middle test\n";
//                for (int i =0; i < n; i++) {
//                    std::cout << arr[i] << " ";
//                }
//                std::cout << "\n";

            },
            [&](std::vector<int> &C) -> void {
//                for(int i = 0; i < n; i++){
//                    //std::cout << "p3\n";
//                    arr[i] = C[i];
//                }

//                       std::cout << "test C\n";
//                       for (int i =0; i < n; i++) {
//                           std::cout << arr[i] << " ";
//                       }
//                       std::cout << "\n";
            });

    // end timing
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elpased_seconds = end-start;

    // display time to cerr
    std::cerr<<elpased_seconds.count()<<std::endl;
    checkMergeSortResult (arr, n);

  #if DEBUG
        for (int i=0; i<n; ++i)
        std::cout<<arr[i]<<" ";
      std::cout<<std::endl;
  #endif

  delete[] arr;

  return 0;
}



//int curr_size;  // For current size of subarrays to be merged
//                // curr_size varies from 1 to n/2
//                int left_start; // For picking starting index of left subarray
//                // to be merged
//
//                // Merge subarrays in bottom up manner.  First merge subarrays of
//                // size 1 to create sorted subarrays of size 2, then merge subarrays
//                // of size 2 to create sorted subarrays of size 4, and so on.
//                for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
//                {
//                    // Pick starting point of different subarrays of current size
//                    for (left_start=0; left_start<n-1; left_start += 2*curr_size)
//                    {
//                        // Find ending point of left subarray. mid+1 is starting
//                        // point of right
//                        int mid = std::min(left_start + curr_size - 1, n-1);
//
//                        int right_end = std::min(left_start + 2*curr_size - 1, n-1);
//
//                        // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
//                        //std::lock_guard<std::mutex> lck (mut);
//                        merge(std::ref(arr), left_start, (mid+1), right_end);
//                    }
//                }



//void MergeSort(int t, int p, int r, int *a);
//void Merge(int *A,int *L,int leftCount,int *R,int rightCount);
//void MergePar(int t, int p1, int r1, int p2, int r2,
//              int* a, int p3 );
//
//
//void MergeSort(int *A, int n) {
//    int mid,i, *L, *R;
//    if(n < 2) return;
//    mid = n/2; // find the mid index.
//    L = (int*)malloc(mid*sizeof(int));
//    R = (int*)malloc((n - mid)*sizeof(int));
//    for(i = 0;i<mid;i++) L[i] = A[i];
//    for(i = mid;i<n;i++) R[i-mid] = A[i];
//    MergeSort(L,mid); // sorting the left subarray
//    MergeSort(R,n-mid); // sorting the right subarray
//    Merge(A,L,mid,R,n-mid);
//}
//
//void MergeSort(int t, int p, int r, int *a) {
//    int mid, i, n;
//    n = r-p;
//    if(n < 2) return;
//    mid = n/2; // find the mid index.
//    MergeSort(t, p, mid, a); // sorting the left subarray
//    MergeSort(t, mid+1, r, a); // sorting the right subarray
//    Merge(t, p, mid, mid+1, r, a, p);
//}
//
//void Merge(int *A,int *L,int leftCount,int *R,int rightCount) {
//    int i,j,k;
//// i - to mark the index of left subarray (L)
//// j - to mark the index of right subarray (R)
//// k - to mark the index of merged subarray (A)
//    i = 0; j = 0; k =0;
//    while(i<leftCount && j< rightCount) {
//        if(L[i] < R[j]) A[k++] = L[i++];
//        else A[k++] = R[j++];
//    }
//    while(i < leftCount) A[k++] = L[i++];
//    while(j < rightCount) A[k++] = R[j++];
//}
//
//void MergePar(int t, int p1, int r1, int p2, int r2,
//              int* a, int p3 )
//{
//    int length1 = r1 - p1 + 1;
//    int length2 = r2 - p2 + 1;
//    if ( length1 < length2 )
//    {
//        std::exchange(p1, p2 );
//        std::exchange(r1, r2 );
//        std::exchange(length1, length2 );
//    }
//    if ( length1 == 0 ) return;
//    int q1 = ( p1 + r1 ) / 2;
//    int q2 = BinarySearch( t[ q1 ], t, p2, r2 );
//    int q3 = p3 + ( q1 - p1 ) + ( q2 - p2 );
//    a[ q3 ] = t[ q1 ];
//    MergePar( t, p1, q1 - 1, p2, q2 - 1, a, p3 );
//    MergePar( t, q1 + 1, r1, q2, r2, a, q3 + 1 );
//}


//main
//    int i;
//    float a[n], b[n], c[n];
//    for (i=0; i < n; i++)
//        a[i] = b[i] = c[i];
//
//    # pragma omp parallel shared(a,b,c) private(i)
//        {
//
//    # pragma omp sections
//            {
//    # pragma omp section
//                {
//                    for (i=0; i < n/2; i++) c[i] = a[i] + b[i];
//                }
//    # pragma omp section
//                {
//                    for (i=n/2; i < n; i++) c[i] = a[i] + b[i];
//                }
//            } /* end of sections */
//
//        } /* end of parallel */
//
//    # pragma omp parallel
//    {
//    # pragma omp sections
//    {
//    # pragma omp section
//        MergePar( t, p1, q1 - 1, p2, q2 - 1, a, p3 );
//    # pragma omp section
//     MergePar( t, q1 + 1, r1, q2, r2, a, q3 + 1 );
//    } // END sections block
//    } // end parallel block
