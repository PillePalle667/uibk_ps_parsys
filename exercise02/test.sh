#!/bin/bash

g++ $2 -o mmulNV -O3 -DNESTED_VECTOR -std=c++11 -Wall -fopenmp
g++ $2 -o mmulMU -O3 -DCONTIGUOUS_WITH_MULTIPLICATION -std=c++11 -Wall -fopenmp
g++ $2 -o mmulIND -O3 -DCONTIGUOUS_WITH_INDIRECTION -std=c++11 -Wall -fopenmp
echo "BENCHMARKING OF "$2>$1 

for j in 64 128 246 512 1024 
do
echo "=======================================">>$1
	echo "PROBLEM SIZE : $j">>$1
	echo "========">>$1
	echo "Nested Vector">>$1
	(multitime -n 7 ./mmulNV $j)&>>$1
	perf stat ./mmulNV $j &>>$1
	echo "========">>$1
	echo "Multiplication">>$1
	(multitime -n 7 ./mmulMU $j)&>>$1
	perf stat ./mmulMU $j &>>$1 
	echo "========">>$1
	echo "Indirection">>$1
	(multitime -n 7 ./mmulIND $j)&>>$1
	perf stat ./mmulIND $j &>>$1 
	#for i in {1..7}; do 
	#/usr/bin/time --output=$2 --append ./$1 $j >>$2;
	#done;
done;


