#include <cassert>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <vector>
using namespace std;
#include "mpi.h"

void vector_flater_mpi(const double *v1, double *flat, const int count) {
  for (size_t i = 1; i < count; i++) {
    double avg = v1[i - 1] + v1[i + 1];

    avg /= 2;

    flat[i] = avg;
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc != 3) {
    if (rank == 0) {
      // cout << "Usage: " << argv[0] << " <number> <number>" << endl;
    }
    MPI_Finalize();
    return 1;
  }

  int count = atoi(argv[1]);
  int iterations = atoi(argv[2]);

  int remain = count % size;
  double *v1;
  double *flat;
  double *chunk;
  double *res = new double[count];
  int chunk_size;

  double start_time = MPI_Wtime();

  if (rank == 0) {
    int min_chunk_size = count / size + 1;
    v1 = new double[count];
    for (int i = 0; i < count; i++) {
      v1[i] = 0;
    }
    v1[0] = 0;
    v1[1] = 0;
    v1[count - 3] = 100;
    v1[count - 2] = 100;
    v1[count - 1] = 100;

    chunk_size = min_chunk_size;
    if (remain > 0) {
      chunk_size += 1;
    }
    // int first_start = 0;
    int first_end = 0 + chunk_size - 1;
    int prev_end = first_end;

    cout << "remain" << remain << endl;
    cout << "chunk_size " << chunk_size << " first_end " << first_end << endl;

    for (int i = 1; i < size; i++) {
      uint rsv_chunk_size = min_chunk_size + 1;
      uint start;
      uint end;

      if (remain > i) {
        rsv_chunk_size += 1;
      }
      // last
      if (i == (size - 1)) {
        rsv_chunk_size = rsv_chunk_size - 1;
        // assert(rsv_chunk_size == 5);
        start = prev_end - 2;
        end = count - 1;
      } else {
        // middle
        start = prev_end - 1;
        end = start + rsv_chunk_size;
      }
      prev_end = end;
      assert(end < count);

      cout << "i " << i << " chunk_size " << chunk_size << " start " << start
           << " end " << end << " " << v1[end] << " " << v1[start] << endl;
      MPI_Send(&v1[start], rsv_chunk_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }

    cout << "fet" ;
    chunk = v1;

  } else {
    chunk_size = count / size + 2;
    if (remain > rank) {
      chunk_size += 1;
    }
    if (rank == (size - 1)) {
      chunk_size -= 1;
    }
    chunk = new double[chunk_size];
    // if (rank == 1) {
      cout << "size " << chunk_size << endl;
    // }
    MPI_Recv(chunk, chunk_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    cout << "foo";
  }

  flat = new double[count];
  for (int i = 0; i < count; i++) {
    flat[i] = 0;
  }

  for (int n = 0; n < iterations; n++) {
    vector_flater_mpi(chunk, flat, chunk_size);
    auto temp = &chunk;
    chunk = flat;
    flat = *temp;
  }

  int *recvcounts = new int[size];
  int *displs = new int[size];

  for (int i = 0; i < size; i++) {
    recvcounts[i] = (chunk_size - 1) + (i < remain ? 1 : 0);

    displs[i] = (i == 0) ? 0 : displs[i - 1] + recvcounts[i - 1] - 1;
  }

  if (rank == 0) {
    int gather_size = chunk_size - 1;
    // cout << "rank " << rank << " gather_size " << gather_size << endl;
    // for (int i = 0; i < size; i++) {
    //   cout << "res" << i << " " << recvcounts[i] << " " << displs[i] << endl;
    // }
    MPI_Gatherv(&chunk[0], gather_size, MPI_DOUBLE, res, recvcounts, displs,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    int gather_size = (rank == (size - 1)) ? chunk_size - 1 : chunk_size - 2;
    // cout << "rank " << rank << " gather_size " << gather_size << endl;
    MPI_Gatherv(&chunk[1], gather_size, MPI_DOUBLE, res, recvcounts, displs,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  double end_time = MPI_Wtime();
  if (rank == 0) {
    for (int i = 0; i < count; i++) {
      cout << res[i] << " ";
    }
    cout << endl;
    cout << size << " " << count << endl;
    cout << end_time - start_time << endl;
  }

  MPI_Finalize();
  return 0;
}
