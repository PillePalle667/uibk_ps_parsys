#!/bin/bash
#$ -N Stencil2DBenchmark
#$ -q std.q
#$ -cwd
#$ -o s2dsec.txt
#$ -e error.txt
#$ -pe openmp 8


max=$((max + 0))
y=$((y + 0))
min=$((min + 0))
echo "Unoptimised " 
for i in {1..7}
do
	read -ra values <<< `./S2D`
	x=${values[0]}
	if [ $x -gt $max ]
	then 
	   max=$((x))
	fi 
	if [ $i == 1 ]
	then
	   min=$((x))
	fi
	if [ $min -gt $x ]
	then  
	  min=$((x))
	fi
	y=$((y + x))
done;
y=$((y / 7))
echo "Mean: " $y
echo "MAX: " $max
echo "MIN: " $min



max=$((0 + 0))
y=$((0 + 0))
min=$((0 + 0))
echo "Optimised"
echo "1 Threads"
for i in {1..7}
do
	read -ra values <<< `./S2DOpt`
	x=${values[0]}
	if [ $x -gt $max ]
	then 
	   max=$((x))
	fi 
	if [ $i == 1 ]
	then
	   min=$((x))
	fi
	if [ $min -gt $x ]
	then  
	  min=$((x))
	fi
	y=$((y + x))
done;
y=$((y / 7))
echo "Mean: " $y
echo "MAX: " $max
echo "MIN: " $min
perf stat ./S2D
perf stat ./S2DOpt


