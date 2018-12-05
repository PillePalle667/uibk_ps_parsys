#!/bin/bash

g++ MergeSort.cpp -o merge -O3 -std=c++11 -Wall
g++ MergeSortUNIT.cpp -o mergeUNIT -O3 -std=c++11 -Wall 

echo "UNIT TEST MERGESORT" 
./mergeUNIT m
./mergeUNIT s
./mergeUNIT p
./mergeUNIT b
echo "KTHXBYE"
