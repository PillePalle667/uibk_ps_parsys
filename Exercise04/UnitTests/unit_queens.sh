#!/bin/bash

g++ Queens.cpp -o queens -O3 -std=c++11 -Wall
g++ QueensUNIT.cpp -o queensUNIT -O3 -std=c++11 -Wall 

echo "UNIT TEST NQUEENS" 
./queensUNIT s
./queensUNIT i

echo "KTHXBYE"
