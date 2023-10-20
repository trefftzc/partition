#
# Program that solves the partition problem in python
# Sequential version
#

def evaluatePartition(  value,  n, array) :
   sum0s = 0
   sum1s = 0
   mask = 1
   for i in range(0,n):
     if ((mask & value) != 0):
       sum1s = sum1s + array[i]
     else:
       sum0s = sum0s + array[i]
     mask = mask * 2

   if (sum0s == sum1s):
     return 1
   else:
     return 0

def printResults(value, n, array):
  print("Solution:\n")
  print("First partition: ")
  mask = 1
  sum = 0
  for i in range(0,n):
    if ((mask & value) != 0):
      print(array[i]," ")
      sum = sum + array[i]
    mask = mask * 2

  print(" sum: ",sum)
  print("Second partition: ")
  mask = 1
  sum = 0
  for i in range(0,n):
    if ((mask & value) == 0):
      print(array[i]," ")
      sum = sum + array[i]
    mask = mask * 2
  print(" sum: \n",sum)





if __name__ == "__main__":

  # n is the size of the array with the integer values
  # array contains the set of integer values
  nAsString = input()
  n = int(nAsString)
  print("The value of n is \n",n)

  array=[]
  # If one were to read all the values from the next line
  # array =  [int(item) for item in input().split()]
  for i in range(0,n):
    valueAsString = input()
    value = int(valueAsString)
    array.append(value)

  print("The read values are: \n")
  for i in range(0,n):
    print(array[i]," ") 
		
  print("\n")

  nPartitions = 1
  # Only half of all possible partitions need be examined
  # The second half is symmetrical to the first half
  for i in range(1,n):
    nPartitions = nPartitions * 2

  print("The number of possible partitions is: ",nPartitions)

  solutionFound = 0
  solution = -1
  for i in range(1,nPartitions):
    if (evaluatePartition( i,  n, array) == 1):
      solutionFound = 1
      solution = i
      break
			
  if (solutionFound):
    printResults(solution, n, array)
  else:
    printf("No solution was found.")
