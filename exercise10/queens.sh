#!/bin/bash

#$ -N mpi_test
#$ -q std.q
#$ -cwd
#$ -o mpi_queens.out
#$ -e mpi_queens.err

# exclusively reserve whatever nodes this is running on
#$ -l excl=true

# reserve 4GB of memory per process
#$ -l h_vmem="4G"

#$ -pe openmpi-8perhost 64

module load gcc/8.2.0
module load openmpi/3.1.1
echo '========'

mpirun -np 2 ./queens
mpirun -np 4 ./queens
mpirun -np 8 ./queens
mpirun -np 16 ./queens
mpirun -np 32 ./queens
mpirun -np 64 ./queens


module purge
