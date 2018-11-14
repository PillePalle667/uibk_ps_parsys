#!/bin/bash

g++ $2 -o algo -O3 
g++ $2 -o algoOpt -O3 -fopenmp

echo "BENCHMARKING OF "$2>$1 

#arrayN=(10 11 12 13 14)
#arrayN=(10 10000 1000000 10000000)
arrayN=(100 1000 10000 100000 1000000)
for j in "${arrayN[@]}"
do
echo "======================================= ">>$1
	echo "PROBLEM SIZE : $j">>$1
	
	echo "========">>$1
	echo "$2">>$1
	(multitime -n 7 ./algo $j)&>>$1
	perf stat ./algo $j &>>$1
echo "======================================= OPENMP">>$1
	echo "PROBLEM SIZE : $j">>$1
	
	echo "========">>$1
	echo "$2">>$1
	(multitime -n 7 ./algoOpt $j)&>>$1
	perf stat ./algoOpt $j &>>$1

done;
