#!/bin/bash

# build
g++ -fopenmp -o omp omp.cpp
g++ -fopenmp -o default default.cpp
# default
# for elements in 100000 1000000 2000000; do
#   hyperfine --runs 10 --warmup 1 "./default $elements" --shell=none --export-json "./results/default/default-elements-$elements.json"
# done

mpic++ mpi.cpp -o mpi -g

touch mpi.txt
# openmp
for threads in  1 2 3 4 5 6 7 8; do
  echo " " >> mpi.txt
  for elements in 100000 1000000 2000000; do
    mpirun -n $threads --oversubscribe ./mpi $elements 2 >> mpi.txt
  done
done
