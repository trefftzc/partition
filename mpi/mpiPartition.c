/*
 * mpiPartition.c
 * Solve the Partition problem using mpi
 * https://en.wikipedia.org/wiki/Partition_problem
 * For an introduction to MPI, visit:
 * https://computing.llnl.gov/tutorials/mpi/
 * To execute:
 * mpiexec -np 4 mpiPartition < testfile1 
 * This code works for multisets of up to 32 elements
 * The input is expected to be as follows:
 * The first line will contain n, the number of elements in the multiset
 * The remaining n lines will contain the n values, one per line
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int evaluatePartition( unsigned int value, int n, int *array) {
  int sum0s = 0;
  int sum1s = 0;
  unsigned int mask = 1;
  for(int i = 0;i < n;i++) {
    if ((mask & value) != 0) {
      sum1s = sum1s + array[i];
    }
    else {
      sum0s = sum0s + array[i];
    }
    mask = mask * 2;
  }
  if (sum0s == sum1s)
     return 1;
  else
     return 0;
}


void printResults(unsigned int value,int n,int *array) 
{
  printf("Solution:\n");
  printf("First partition: ") ;
  unsigned int mask = 1;
  int sum = 0;
  for(int i = 0;i < n;i++) {
    if ((mask & value) != 0) {
      printf("%d ",array[i]);
      sum = sum + array[i];
    }
    mask = mask * 2;
  }
  printf(" sum: %d \n",sum);
  printf("Second partition: ") ;
  mask = 1;
  sum = 0;
  for(int i = 0;i < n;i++) {
    if ((mask & value) == 0) {
      printf("%d ",array[i]);
      sum = sum + array[i];
    }
    mask = mask * 2;
  }
  printf(" sum: %d \n",sum);
}



int main(int argc,char *argv[]) {

  MPI_Status status;
  int myRank;
  int numProcesses;

  int n;
  int *array;

  // Initialize all the nodes
  // Find the rank of every node
  // Find the total number of processes that are active
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
  // Only the master node reads the data
  if (myRank == 0) {
    scanf("%d",&n);

    printf("The value of n is %d\n",n);
    array = (int *) malloc (n * sizeof(int));
    for(int i = 0;i < n;i++) {
      scanf("%d",&array[i]);
    }
    printf("The read values are: \n");
    for(int i = 0;i < n;i++) {
      printf("%d ",array[i]);
    }
    printf("\n");
  }
  // Then the master node broadcasts the input data
  // to all other nodes
  // First the size of the array
  // and then the array
  MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
  if (myRank != 0) {
    array = (int *) malloc (n * sizeof(int));
  }
  MPI_Bcast(array,n,MPI_INT,0,MPI_COMM_WORLD);


  unsigned int nPartitions = 1;
  for(int i = 0;i < n;i++) {
    nPartitions = nPartitions * 2;
  }
  //printf("The number of possible partitions is: %d\n",nPartitions);
  // Only half of all possible partitions need be examined
  // The second half is symmetrical to the first half
  nPartitions = nPartitions / 2;

  int solutionFound = 0;
  int solution = -1;

  // Every node will work on a different portion of the
  // set of candidates
  int portionEachNode = nPartitions / numProcesses;


  int initialPartition = portionEachNode * myRank;
  int finalPartition;
  if (myRank != (numProcesses-1)) {
      finalPartition = initialPartition + portionEachNode;
    }
    else {
      finalPartition = nPartitions;
    }

  for(unsigned int i = initialPartition;i < finalPartition;i++) {
    if (evaluatePartition( i,  n, array) == 1) {
      solutionFound = 1;
      solution = i;
      break;
    }
  }

  // Find the minimum across all nodes

  int minAllNodes;
  MPI_Reduce(&solution,&minAllNodes,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

  if (myRank == 0) { 
    if (minAllNodes != -1) {
      printResults(minAllNodes, n, array);
    }
    else {
      printf("No solution found.\n");
    }
  }


  MPI_Finalize();
  return 0;
}
 
