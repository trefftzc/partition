## Partition
Brute force code that finds if an instance of the partition problem has a solution or not

This text was originally here: https://faculty.computing.gvsu.edu/trefftzc/partition/partition.html

The partition problem is intuitive: Given a collection of n objects, can we place part of those objects in the left hand of a balance and the remaining objects in the right hand side of a balance so that they weigh the same? In other (more mathematical) words, if we have a multiset of integers s, can we partition that multiset into two separate multisets q and r so that the sum of the elements in q is the same as the sum of the elements in r. A more formal statement of the problem can be found on mathworld.wolfram: https://mathworld.wolfram.com/NumberPartitioningProblem.html

This simple problem belongs to a very unique kind of problems called NP-complete problems that are very hard to solve.

Given an instance of the partition problem with n positive integer values, one way to find if there is a solution to that instance is to examine each and every one of the 2n subsets and to check for every one of those subsets if that subset and its complement are a solution to this instance of the partition problem. One adds up the elements in the subset, adds up the elements in the complement and if the two sums are the same, this is a solution to this instance of the partition problem.

Consider the following example: {2,3,5}. In this case n, the number of elements in the instance of the partition problem, is 3. If one calculates all the possible subsets, there are 2 3 = 8 possible distinct subsets. The following table shows the eight subsets, their complements and indicates which of those subsets are solutions for this instance of the partition problem.


| Index	| Binary Encoding	|  Elements	| Elements in the Complement	|  Solution? |
|-------|-----------------|-----------|-----------------------------|------------|
|0	    | 000	|{}	    |{2,3,5}	|No | 
|1	    | 001	|{2}	  |{3,5}	  |No |
|2	    | 010	|{3}	  |{2,5}	  |No |
|3	    | 011	|{2,3}  |{5}	    |Yes|
|4	    | 100	|{5}	  |{2,3}	  |Yes|
|5	    | 101	|{2,5}  |{3}	    |No |
|6	    | 110	|{3,5}  |	{2}	    |No |
|7	    | 111	|{2,3,5}|	{}	    |No |

Notice that the first half of the table is symmetrical to the second half of the table. Hence, one only needs to go through the first half of all the elements in the Power Set.

On this repository you will find several programs that solve this problem. The input file has the following format:

The first line contains the number of integers in the multiset, n
The next n lines contain the n integers, one per line.

The sequential version of the program is called sequentialPartition.c 
This version works only for small instances of the problem, it will solve instances of size at most 32. 
This version uses "brute force".

There is a Makefile available that will allow you to compile the code.  You can compile the code by typing:

make sequentialPartition

You can execute the program (after compiling it) by typing:

./sequentialPartition < test16.txt

If you want to find how long does the program take to execute, you can use the command time as follows:

time ./sequentialPartition < test16.txt

If you observe carefully the source code of that solution, and furthermore if you profile the execution of the code, you will notice that the loop in the main function is responsible for the bulk of the execution time of the code.

## OpenMP
That loop can be parallelized using OpenMP, a standard that allows developers to parallelize programs written in C/C++ or Fortran. OpenMP is available with gcc. A parallel version of the sequential code based on OpenMP is available: openmpPartition.c Again you can compile this version of the program by typing:

make openmpPartition

The changes to the source code are minimal:
1. Including the header file for the openmp library.
2. A pragma statement that indicates to the compiler to parallelize the main loop.

As with the sequential program, you can compile this version of the program by typing:

make openmpPartition

You can execute the program and observe the speedup by typing:

time ./openmpPartition < test16.txt

Computers with microprocessors with several cores are ubiquitous these days. OpenMP is available on the gcc family of compilers and on Visual Studio. Lawrence Livermore National Laboratory has an excellent tutorial on OpenMP that is available here: https://hpc-tutorials.llnl.gov/openmp/
