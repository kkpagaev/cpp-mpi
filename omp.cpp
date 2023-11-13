#include <cstdio>
#include <iostream>
#include <omp.h>
#include <ostream>
#include <chrono>
#include <string>
#include <vector>
using namespace std;

void vector_flater_omp(const vector<double> *vec,
                       const vector<int> *fixed_indexes, vector<double> *flat) {
#pragma omp parallel for
  for (size_t i = 1; i < vec->size() - 1; i++) {
    bool skip = false;
    for (size_t j = 0; j < fixed_indexes->size(); j++) {
      if (i == (*fixed_indexes)[j]) {
        skip = true;
        break;
      }
    }

    if (!skip) {
      double avg = (vec->at(i - 1) + vec->at(i + 1));

      avg /= 2;

      // std::cout << vec->at(i - 1) << " + " << vec->at(i + 1) <<" index" << i
      // << " avg: " << avg << std::endl;
      (*flat)[i] = avg;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <number> <number>" << endl;
    return 1;
  }

  int count = atoi(argv[1]);
  int threads = atoi(argv[2]);

  omp_set_num_threads(threads);

  vector<double> *v1 = new vector<double>(count);
  vector<int> *fixed = new vector<int>(2);
  (*fixed)[0] = 1;
  (*fixed)[1] = count - 2;

  // cout << fixed;

  vector<double> *flat = new vector<double>(count);
  (*flat)[0] = 101;
  (*flat)[1] = 100;
  // (*flat)[count -11] = 100;
  (*flat)[count - 2] = 100;

  auto t1 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000; i++) {
    vector_flater_omp(v1, fixed, flat);
    auto temp = &v1;
    v1 = flat;
    flat = *temp;
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

  cout << to_string(duration) << endl;


  // for (double f : *flat) {
  //   cout << f << '\n';
  // }

  return 0;
}
