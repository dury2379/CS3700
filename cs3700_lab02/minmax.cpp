#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <math.h>

#define ARRAY_SIZE 1000000

int main (int argc,  char *argv[]) {

   int myid, numprocs;
   int namelen;
   int* numbers = new int[ARRAY_SIZE];
   char processor_name[MPI_MAX_PROCESSOR_NAME];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);
 
   printf("Process %d on %s\n", myid, processor_name);
 
   srand((unsigned)(MPI_Wtime()));
   
   for (int i=0; i<ARRAY_SIZE; i++)
   {
      numbers[i] = (int)((((double)rand()) / ((double)RAND_MAX)) * 1000000.0);  //could be randomly generated
      // if (i % 1000 == 0)
      	// printf("Value %ld is %ld \n", i, numbers[i]);
   }

   int s = (int)floor(ARRAY_SIZE/numprocs);
   int s0 = s + ARRAY_SIZE%numprocs;

   int startIndex = s0 + (myid-1)*s;
   int endIndex = startIndex + s;

   double startwtime;
   if (myid == 0) {
      startwtime = MPI_Wtime();
   }

   int i;
   int partial_min = numbers[startIndex];
   int partial_max = numbers[startIndex];
   
   if (myid == 0) {
      // master worker - comput the master's numbers
      for (i=0; i<s0; i++) {
         if (partial_min > numbers[i])
         {
            partial_min = numbers[i];
         }
         else if (partial_max < numbers[i])
         {
            partial_max = numbers[i];
         }
      }
      printf("Process %d - startIndex 0 endIndex %d; partial min: %ld; partial max: %ld\n",
             myid, s0-1, partial_min, partial_max);
   } else {
      //slave's work
      for (i= startIndex; i<endIndex; i++) {
         if (partial_min > numbers[i])
         {
            partial_min = numbers[i];
         }
         else if (partial_max < numbers[i])
         {
            partial_max = numbers[i];
         }
      }
      printf ("Process %d - startIndex %d endIndex %d; partial min: %ld; partial max: %ld\n",
              myid, startIndex, endIndex-1, partial_min, partial_max);
   }

   int min = numbers[0];
   int max = numbers[0];
   MPI_Reduce(&partial_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
   MPI_Reduce(&partial_max, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

   if (myid == 0) {
      double runTime = MPI_Wtime() - startwtime;
      printf("Execution time (sec) = %f min = %ld max = %ld \n",
             runTime, min, max);
   }

   delete[] numbers;

   MPI_Finalize();
}

