#!/usr/bin/python3.4

from mpi4py import MPI
import numpy
import math
import random

def main():
	world = MPI.COMM_WORLD
	numprocs = world.size
	myid = world.rank
	procname = MPI.Get_processor_name()
	print('Process %d on %s' %(myid, procname))

	NUM_TRAILS = 10 ** 2

	s = NUM_TRAILS // numprocs
	s0 = s + NUM_TRAILS%numprocs

	totalTime = 0
	total = 0
	approx_pi = -1
	delta = 0
	part_sum = None
	startwtime = None

	if myid == 0: # master worker
		startwtime = MPI.Wtime()
		part_sum = run_N_trails(s0)
	
	else: # slave worker
		part_sum = run_N_trails(s)
		print("Subprocess id: %d, hit count: %d, total trails: %d." %(myid, part_sum, s))
	
	total = world.reduce(part_sum, op=MPI.SUM, root=0)
	
	world.barrier()
	
	if myid == 0:
		approx_pi = (total / NUM_TRAILS) * 4
		totalTime = MPI.Wtime() - startwtime
		delta = approx_pi - math.pi
		print("Approximate pi:", approx_pi)
		print("Delta:", delta)
		print("Total time:", totalTime)
		print("Number of trails:", NUM_TRAILS)


def run_N_trails(n):
	random.seed(MPI.Wtime())
	result = 0;
	for i in range(n):		
		x = random.uniform(0,1)
		y = random.uniform(0,1)
		if x ** 2 + y ** 2 <= 1:
			result += 1
	return result

main()
    
