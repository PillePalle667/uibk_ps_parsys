#!/bin/bash
g++ $3 -o $1 -O3 -DNESTED_VECTOR -std=c++11 -Wall -fopenmp
echo "BENCHMARKING OF "$1>$2 
echo "Nested Vector">>$2
for j in 64 128 246 512 1024 
do
echo "=======================================">>$2
	echo "PROBLEM SIZE : $j">>$2
	(multitime -n 7 ./$1 $j)&>>$2
	perf stat ./$1 $j &>>$2 
	#for i in {1..7}; do 
	#/usr/bin/time --output=$2 --append ./$1 $j >>$2;
	#done;
done;

g++ $3 -o $1 -O3 -DCONTIGUOUS_WITH_MULTIPLICATION -std=c++11 -Wall -fopenmp
echo "BENCHMARKING OF "$1>>$2 
echo "Multiplication">>$2
for j in 64 128 246 512 1024 
do
echo "=======================================">>$2
	echo "PROBLEM SIZE : $j">>$2
	(multitime -n 7 ./$1 $j)&>>$2 
	perf stat ./$1 $j &>>$2
	#for i in {1..7}; do 
	#/usr/bin/time --output=$2 --append ./$1 $j >>$2;
	#done;
done;

g++ $3 -o $1 -O3 -DCONTIGUOUS_WITH_INDIRECTION -std=c++11 -Wall -fopenmp
echo "BENCHMARKING OF "$1>>$2
echo "Indirection">>$2 
for j in 64 128 246 512 1024 
do
echo "=======================================">>$2
	echo "PROBLEM SIZE : $j">>$2
	(multitime -n 7 ./$1 $j)&>>$2 
	perf stat ./$1 $j &>>$2
	#for i in {1..7}; do 
	#/usr/bin/time --output=$2 --append ./$1 $j >>$2;
	#done;
done;


