# Exercise 1


1.
GPROF profiles program execution by function.(Number of calls, )
the mmul example without compiler optimization spends 66% of runntime in the actual computing function (operator*).
There are huge amounts (for example 2147483648) of calls to functions that seem to be associated with resizing of vectors. It seems a lot of time is lost to procedures not directly associated with computing a result.
With O3 compiler optimization there is a total of 4 Function calls other than the operator* function, one for intitalizing a matrix and three to a function presumably concerned with vector allocation amoutning to a total of 0.01 seconds

2. 
Our regimen is a script running problem sizes 64, 128, 246, 512 and 1024 10 times each both with O0 and O3 compiler flags
 and using /bin/time to retrieve clock time , cpu time, user time/system calls and page faults.
It is meant to also include gprof calls however this was disabled for now as it seems to cause performance issues.
problem sizes and number of executions is a compromise between meaningful results and runtime.  ###
 
 ###  ENHANCE BECHMARKING USING MULTITIME 
 
 Evaluating the Real time , user time and system time in terms of mean, median and max value :
 
 '$ multitimt -n 10 ...'
 
reports are in folders for different hardware setups

3. For sequential optimization one method of transposing a matrix to enable sequential processing. Performance was mostly improved (mmulOpt.cpp). Antother experiment was conducted with loop unrolling to maybe enable the compiler to paralellize.
this was a complete and utter failure running slower than the original code and in retrospect the idea was rather silly in the first place, really.

