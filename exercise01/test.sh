#!/bin/bash
g++ $3 -o $1 -O0 -std=c++11 -Wall
echo "BENCHMARKING OF "$1>>$2 
echo "O0">>$2
echo "=======================================">>$2
echo "PROBLEM SIZE 64: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 64 >>$2;
done;
#./$1 64
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 64 >>$2;
echo "=======================================">>$2
echo "PROBLEM SIZE 128: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 128 >>$2;
done;
#./$1 128
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 128 >>$2;
echo "=======================================">>$2
echo "PROBLEM SIZE 512: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 512 >>$2;
done;
#./$1 512
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 512 >>$2;
echo "=======================================">>$2
echo "PROBLEM SIZE 1024: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 1024 >>$2;
done;
#./$1 1024
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 1024 >>$2;
#echo "=======================================">>$2
#echo "PROBLEM SIZE 2048: ">>$2
#/usr/bin/time --output=$2 --append  ./$1 2048 >>$2;
#./$1 2048
#gprof $1>>$2
g++ $3 -o $1 -O3 -std=c++11 -Wall
echo "O3">>$2
echo "========================================================================">>$2
echo "PROBLEM SIZE 64: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 64 >>$2;
done;
#./$1 64
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 64 >>$2;
echo "=======================================">>$2
echo "PROBLEM SIZE 128: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 128 >>$2;
done;
#./$1 128
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 128 >>$2;
echo "=======================================">>$2
echo "PROBLEM SIZE 512: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 512 >>$2;
done;
#./$1 512
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 512 >>$2;
echo "=======================================">>$2
echo "PROBLEM SIZE 1024: ">>$2
for i in {1..10}; do 
/usr/bin/time --output=$2 --append ./$1 1024 >>$2;
done;
#./$1 1024
gprof $1>>$2
#valgrind --log-fd=9 9>>$2 ./$1 1024 >>$2;
#echo "=======================================">>$2
#echo "PROBLEM SIZE 2048: ">>$2
#/usr/bin/time --output=$2 --append  ./$1 2048 >>$2;
#./$1 2048
#gprof $1>>$2





