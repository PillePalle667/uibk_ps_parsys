#!/bin/bash
g++ $3 -o $1 -O0 -std=c++11 -Wall -fopenmp
echo "BENCHMARKING OF "$1>$2 
echo "O0">>$2
for j in 64 128 246 512 1024 
do
echo "=======================================">>$2
	echo "PROBLEM SIZE : $j">>$2
	(multitime -n 7 ./$1 $j)&>>$2 
	#for i in {1..7}; do 
	#/usr/bin/time --output=$2 --append ./$1 $j >>$2;
	#done;
done;

g++ $3 -o $1 -O3 -std=c++11 -Wall -fopenmp
echo "O3">>$2
for j in 64 128 246 512 1024
do
echo "========================================================================">>$2
	echo "PROBLEM SIZE : $j ">>$2
	(multitime -n 7 ./$1 $j)&>>$2 
	#for i in {1..7}; do 
	#/usr/bin/time --output=$2 --append ./$1 $j >>$2;
	#done;
done;


