#!/bin/bash

g++ $2 -o algo -O3 
g++ $2 -o algoOpt -O3 -fopenmp

echo "BENCHMARKING OF "$2>$1 

arrayN=(100 1000 10000 100000 100000)

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
	(multitime -n 7 ./algo $j)&>>$1
	perf stat ./algo $j &>>$1

done;
