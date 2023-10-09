#!/bin/bash

# build
g++ -fopenmp -o omp omp.cpp
g++ -fopenmp -o default default.cpp
# default
# for elements in 100000 1000000 2000000; do
#   hyperfine --runs 10 --warmup 1 "./default $elements" --shell=none --export-json "./results/default/default-elements-$elements.json"
# done

# openmp
for threads in  1 2 3 4 5 6 7 8; do
  for elements in 100000 1000000 2000000; do
    hyperfine --runs 10 --warmup 1 "./omp $elements $threads" --shell=none --export-json "./results/$elements/openmp-threads-$threads-elements-$elements.json"
  done
done

cd results 
ls | xargs -I "{}" cat {} | jq '.results[0]' | jq -r ' .command, ((.mean) * 1000)'
echo "command and ms"

