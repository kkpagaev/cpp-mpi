#include <cstdio>
#include <iostream>
#include <ostream>
#include <vector>
using namespace std;


vector<double> vector_flater(const vector<double> vec,  const vector<int> fixed_indexes) {
  vector<double> flat(vec.size() - fixed_indexes.size());
  size_t fixed_index = 0;

  for (size_t i = 1; i < vec.size() - 1; i++) {
    if (fixed_index < fixed_indexes.size() && i == fixed_indexes.at(fixed_index)) {
      fixed_index++;
      continue;
    } 
    double avg = (vec.at(i - 1) + vec.at(i + 1));

    avg /= 2;

    flat[i] = avg;
  }
  
  return flat;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <number>" << std::endl;
    return 1;
  }

  const char* arg = argv[1];
    
  int count = std::atoi(arg);

  vector<double> v1 = vector<double>(count);
  vector<int> fixed = {1,3};
  vector<double> flat = vector_flater(v1, fixed);

  return 0;
}
