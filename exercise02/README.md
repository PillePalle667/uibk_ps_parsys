# Parallel System
PS Parallel System

Authors: Bernhard, Karen, Zahra

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
 


### Using pref on benchmarking versions

Benchmarking by using multitime for having min, max and mean value on three versions. 
Then running  'pref ' on three benchmarking versions:

`perf stat multitime ./test2.sh ...`
  ### Output for Celeron Duel-Core CPU:
  
  
`=====> multitime Result`

```

  Mean        Std.Dev.    Min         Median      Max
real        0.001       0.000       0.001       0.001       0.001       
user        0.001       0.000       0.001       0.001       0.001       
sys         0.000       0.000       0.000       0.000       0.000       


```

|         | multitime | Preformance Counter | 

|--------| ------------------|------------------|
| <ul><li>[x] </li> | 
```

  Mean        Std.Dev.    Min         Median      Max
real        0.001       0.000       0.001       0.001       0.001       
user        0.001       0.000       0.001       0.001       0.001       
sys         0.000       0.000       0.000       0.000       0.000       


``` | 
```
   1.55 msec task-clock                #    0.733 CPUs utilized          
                 1      context-switches          # 1000.000 M/sec                  
                 0      cpu-migrations            #    0.000 K/sec                  
               115      page-faults               # 115000.000 M/sec                
         3,203,345      cycles                    # 3203345.000 GHz                 
         1,344,140      instructions              #    0.42  insn per cycle         
           263,586      branches                  # 263586000.000 M/sec             
     <not counted>      branch-misses                                                 (0.00%)

       0.002118546 seconds time elapsed

       0.002173000 seconds user
       0.000000000 seconds sys


```