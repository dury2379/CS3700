#!/bin/bash
#SBATCH --job-name=COMPILE_SUM_CPP       # Job name
#SBATCH --output=COMPILE_SUM_CPP_%j.log  # Log file name
#SBATCH --partition=compute              # Use computing cluster
#SBATCH --mem=1gb                        # Job memory request
#SBATCH --nodes=1                        # Number of computing nodes
#SBATCH --time=00:02:00                  # Time limit HH:MM:SS

. /etc/profile.d/modules.sh

module load openmpi/2.1.2

/opt/openmpi-2.1.2/bin/mpirun g++ -o sum sum.cpp -I /opt/openmpi-2.1.2/include -L /opt/openmpi-2.1.2/lib -l mpi

