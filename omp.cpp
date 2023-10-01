#include <cstdio>
#include <iostream>
#include <omp.h>
#include <ostream>
#include <vector>
using namespace std;


vector<double> vector_flater_omp(const vector<double> vec,  const vector<int> fixed_indexes) {
  vector<double> flat(vec.size() - fixed_indexes.size());
  size_t fixed_index = 0;
  size_t i = 1;

  #pragma omp parallel for private(fixed_index)
  for (i = 1; i < vec.size() - 1; i++) {
    if (fixed_index < fixed_indexes.size() && i == fixed_indexes.at(fixed_index)) {
      fixed_index++;

      continue;
    } else {
      double avg = (vec.at(i - 1) + vec.at(i + 1));

      avg /= 2;

      flat[i] = avg;
    }
  }

  return flat;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <number> <number>" << std::endl;
    return 1;
  }

  int count = std::atoi(argv[1]);
  int threads = std::atoi(argv[2]);

  omp_set_num_threads(threads);

  vector<double> v1    = vector<double>(count);
  vector<int>    fixed = {1,3};
  vector<double> flat  = vector_flater_omp(v1, fixed);

  return 0;
}
