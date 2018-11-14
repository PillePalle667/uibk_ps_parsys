#!/bin/bash

g++ MergeSort.cpp -o merge -O3 -std=c++11 -Wall
g++ MergeSortUNIT.cpp -o mergeUNIT -O3 -std=c++11 -Wall 

echo "UNIT TEST Bubble" 
./mergeUNIT m
./mergeUNIT s
./mergeUNIT p
./mergeUNIT b
echo "result $?"
