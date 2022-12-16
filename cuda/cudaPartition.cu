/*
 * cudaPartition.cu
 * Solve the Partition problem using CUDA.
 * https://en.wikipedia.org/wiki/Partition_problem
 * This code works for multisets of up to 32 elements
 * The input is expected to be as follows:
 * The first line will contain n, the number of elements in the multiset
 * The remaining n lines will contain the n values, one per line
 */
#include <stdio.h>
#include <stdlib.h>
// The kernel
// This function is executed, in parallel, on the processors on the GPU card
//
__global__
void evaluatePartition(  int n, int *array,int *result) {
  unsigned int value = blockIdx.x*blockDim.x + threadIdx.x;
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
     result[value] = 1;
  else
     result[value] = 0;
  // printf("%d %d \n",value,result[value]);
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


int main() {

  int n;
  int *array;

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

  unsigned int nPartitions = 1;
  for(int i = 0;i < n;i++) {
    nPartitions = nPartitions * 2;
  }
  // printf("The number of possible partitions is: %d\n",nPartitions);
  // Only half of all possible partitions need be examined
  // The second half is symmetrical to the first half
  nPartitions = nPartitions / 2;

  int solutionFound = 0;
  int solution = -1;
  // Allocate the variables in the device:
  // The array with the integer values in the device is called d_array
  int *d_array;
  cudaMalloc(&d_array, n*sizeof(int));

  // Copy the variables from the host to the device
  cudaMemcpy(d_array,array,n*sizeof(int), cudaMemcpyHostToDevice);
 
  // Allocate on the device an array to keep all the results
  int *d_results;
  cudaMalloc(&d_results,nPartitions*sizeof(int));
// Now invoke the kernel
  evaluatePartition<<<(nPartitions+31)/32,32>>>(  n, d_array,d_results) ;
  // The array on the host that will contain the results is called results
  int *results;
  results = (int *) calloc (nPartitions , sizeof(int));
// Copy the results from the GPU card to main memory on the host
  cudaMemcpy(results,d_results,nPartitions*sizeof(int),cudaMemcpyDeviceToHost);
  /*
  for(int i = 0;i < nPartitions;i++) {
	 printf("%d ",results[i]);
  }
  printf("\n");
 */ 
  for(int i = 0;i < nPartitions;i++) {
	  if (results[i] != 0) {
		  solutionFound = 1;
		  solution = i;
		  break;
	  }
  }
   
  if (solutionFound == 1) {
    printResults(solution, n, array);
  }
  else {
    printf("No solution was found.");
  }
  return 0;
}
 
