PS Parallel System

Authors: Bernhard Behr, Karen Errou, Zahra Jafari
Exercise02.
Team: A

### Part 1

Nested Vector: Nested vector have some disadvantages:    
-- The allocation needed is the number of rows times instead of 1.  
-- Access an element takes longer.    
This is why having a 1D vector is better.  
  
Contiguous multiplicaton: It offers a 1 dimension Matrix multiplication which is faster than 2D (Nested Vector) since it consumes less memory and allocation and deallocation overhead.  
  
Contiguous with indirection: Contiguous with indirection is 2 1D vectors the first pointing to the main 1D.  
  
In terms of performance we would expect that the nested vector will take the more memory than Contigous multiplication wth or without indirection.  
The Contigous multiplication will take less memory but is the slowest in term of performance.  
And finally, the contiguous multiplication with indirection is the fastest.   

### Benchmarking  

`perf stat ./benchmark.sh `  

Our benchmark gives the mean, min, max .. of real, user and system time for 7 execution for each size.  
Sizes used are: 64, 128, 246, 512, 1024.  
It executes for each size the 3 versions of cpp.  
The benchmark gives also the perf stat of each version.  

We benchmarked on 3 machines:    
1.  AMD_A8-7410_APU 
2. i7-6500U_CPU_2.50GHz  
3.  Celeron_DualCoreCPU
 

Our benchmark is the file in Code folder: benchmark.sh.  
Our benchmark output, file in each machine-name folder: mmul.txt.  
Execute as follows: ` ./benchmark.sh mmul.txt mmul.cpp `  (Where mmul.txt is the result text and mmul.cpp is the cpp file)

### Part 2

Our Optimized version in Code folder folder: mmulOpt.cpp.  
Our Benchmarked Output, is the file in each machine-name folder: mmulOpt.txt.  

Our Parallelized version iCode folder folder: mmulOpenMP.cpp.  
Our Benchmarked Output, is the file in each machine-name folder: mmulOpenMP.txt.  

