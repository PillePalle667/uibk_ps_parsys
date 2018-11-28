#!/bin/bash
#$ -N AVLBenchmark
#$ -q std.q
#$ -cwd
#$ -o AVL.txt
#$ -e error.txt


for j in 50000 100000 150000 1000000
do
max=$((max + 0))
mean=$((mean + 0))
min=$((min + 0))
echo "Unoptimised size: " $j
for i in {1..7}
do
	read -ra values <<< `./AVL $j`
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
done;



for j in 50000 100000 150000 1000000
do
max=$((0 + 0))
y=$((0 + 0))
min=$((0 + 0))
echo "Optimised size: " $j
for i in {1..7}
do
	read -ra values <<< `./AVLOMP $j`
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
done;


