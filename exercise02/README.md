# Parallel System
PS Parallel System

Authors: Bernhard, Karen Errou, Zahra jafari

Team: A

### Part 1

1. Nested vector is pretty good, if data is only ever added at the end if top level, otherwise is inefficiencies during construction
and run-time memory footprint and should be avoided if possible.
Contiguous is allocation method which each file occupies a set of consecutive addresses on disk.
Contiguous with indirection use to address disk space to large files and allow the greatest flexibility for file sizes and
the fastest retrieval time.
Contiguous with multiplication : Regarding to allocation partition, there are holes which are scattered throughout 
memory. Searching for best and first fit allocation in terms of speed and storage utilization are iterated for each allocation.
So the time of searching for storage allocation is issue.
 

2. ### Using pref on benchmarking versions


Benchmarking by using multitime for having min, max and mean value on three versions. 
Then running  'pref ' on three benchmarking versions:

`perf stat multitime ./test2.sh ...`

  ### Result for Celeron Duel-Core CPU:
  
  
###NESTED_VECTOR
  
`=====> multitime Result`

```
   Mean        Std.Dev.    Min         Median      Max
real        0.001       0.000       0.001       0.001       0.001       
user        0.001       0.000       0.001       0.001       0.001       
sys         0.000       0.000       0.000       0.000       0.000       

 Performance counter stats for :

              1.56 msec task-clock                #    0.671 CPUs utilized          
                 2      context-switches          # 2000.000 M/sec                  
                 0      cpu-migrations            #    0.000 K/sec                  
               117      page-faults               # 117000.000 M/sec                
         3,213,561      cycles                    # 3213561.000 GHz                 
         1,351,723      instructions              #    0.42  insn per cycle         
           265,206      branches                  # 265206000.000 M/sec             
            13,970      branch-misses             #    5.27% of all branches        

       0.002325283 seconds time elapsed

       0.002158000 seconds user
       0.000000000 seconds sys

```

 ###CONTIGUOUS_WITH_INDIRERCTION `
 
  ```
  ===> multitime results:
  
  1: ./test2.sh mmulInd mmulInd.txt mmul.cpp
  
            Mean        Std.Dev.    Min         Median      Max
  real        0.001       0.000       0.001       0.001       0.001       
  user        0.000       0.000       0.000       0.000       0.000       
  sys         0.000       0.000       0.000       0.000       0.000       
  
   Performance counter stats for :
  
                1.77 msec task-clock                #    0.663 CPUs utilized          
                  18      context-switches          # 18000.000 M/sec                 
                   0      cpu-migrations            #    0.000 K/sec                  
                 117      page-faults               # 117000.000 M/sec                
           3,556,424      cycles                    # 3556424.000 GHz                 
           1,446,256      instructions              #    0.41  insn per cycle         
             284,686      branches                  # 284686000.000 M/sec             
              14,880      branch-misses             #    5.23% of all branches        
  
         0.002664710 seconds time elapsed
  
         0.002410000 seconds user
         0.000000000 seconds sys
  
            
 ```
 
 
  ###CONTIGUOUS_WITH_MULTIPLICATION 
  
  ```
 ===> multitime results: 
 
    Mean        Std.Dev.    Min         Median      Max
 real        0.001       0.000       0.001       0.001       0.001       
 user        0.000       0.000       0.000       0.000       0.000       
 sys         0.000       0.000       0.000       0.000       0.000       
 
  Performance counter stats for :
 
               1.77 msec task-clock                #    0.638 CPUs utilized          
                 16      context-switches          # 16000.000 M/sec                 
                  0      cpu-migrations            #    0.000 K/sec                  
                115      page-faults               # 115000.000 M/sec                
          3,596,744      cycles                    # 3596744.000 GHz                 
          1,419,402      instructions              #    0.39  insn per cycle         
            279,360      branches                  # 279360000.000 M/sec             
             14,524      branch-misses             #    5.20% of all branches        
 
        0.002774719 seconds time elapsed
 
        0.002501000 seconds user
        0.000000000 seconds sys
 
 


```
 