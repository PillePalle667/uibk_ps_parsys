#!/bin/bash

#$ -N mpi_test
#$ -q std.q
#$ -cwd
#$ -o mpi_test.out
#$ -e mpi_test.err

# exclusively reserve whatever nodes this is running on
#$ -l excl=true

# reserve 4GB of memory per process
#$ -l h_vmem="4G"

#$ -pe openmpi-8perhost 64

module load gcc/8.2.0
module load openmpi/3.1.1

mpirun -np 1 ./stencil
mpirun -np 2 ./stencil
mpirun -np 4 ./stencil
mpirun -np 8 ./stencil
mpirun -np 16 ./stencil
mpirun -np 32 ./stencil
mpirun -np 64 ./stencil
module purge
