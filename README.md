# Concurrency Exploration
Compare the performance of using different synchronization primitives, gain experience implementing a barrier primitive, and better understand the performance tradeoffs made by my barrier implementation.

We are implementing an inclusive prefix scan algorithm. This is similar to a prefix sum algorithm where the sum is a single instance of the associative and commutative operator that the scan requires. A scan returns the whole set of numbers from the sum, whereas the sum is returning only the final value.

Inclusive vs exclusive pertains to whether the xi of yi = sum(0-xi) is included, in inclusive it is whereas in exclusive it is not.

First, get familiar with the prefix scan algorithm then familiar with the given code.
The code contains a working example of a sequential prefix scan algorithm
We will be implementing a work-efficient version of the algorithm
For each step, we will be comparing the performance of the prefix scan implementations for different parameters over the input files generated. The given python file 'run_tests.py' will run the algorithm while changing parameters.
The correctness of the algorithm can be tested against the sequential implementation that has been provided.

## Part 1
Write a work-efficient parallel prefix scan using pthread barriers.

For each of the provided input files set the number of loops of the operator to 100 000, and graph the execution time of the parallel implementation over the sequential prefix scan implementation as a function of the number of worker threads used. Vary threads from 2 to 32 in increments of 2.
Explain the trends in the graph, why do these trends occur? "A prefix sum algorithm is work-efficient if it does asymptotically no more work (add operations, in this case) than the sequential version. In other words the two implementations should have the same work complexity, O(n)."

## Part 2
Change the loop count to 10, and plot the graphs as before. What happened and why? Vary the -l argument and find the inflexion point (the point at which the sequential and parallel elapsed times meet (Not exact just similar)). Why can changing the number of loops make the sequential version faster than the parallel? What is the most important characteristic of it that makes this happen? Argue in a general way, as if different -l parameters were different operators, after all, -l parameters are just away to quickly create an operator that does something different.
## Part 3
Build a barrier. It's recommended to base your barrier implementation on pthread's spinlocks, but you could use other implementations if you wish. How is your implementation different from pthread barriers? In what scenario(s) would each implementation perform better than the other? What are the pathological cases for each? Repeat the measurements from part 2, graph them, and explain the trends in the graph. Why do these occur? What overheads cause your implementation to underperform an "ideal" speedup ratio?

How do the results from part 2 and part 3 compare? Are they in line with your expectations? Suggest some workload scenarios which would make each implementation perform worse than the other.


## Inputs/Outputs
-n number of threads where 0 means sequential
-i absolute path to input file
-o absolute path to output file
-l number of loops that the operator will do, changing the complexity of the operator.
-s use implemented barrier implementation, else use pthreads barrier

The first line in the input file contains a single integer n which denotes the number of lines of input in the remainder of the input file, the following n lines will each contain a single integer
'generate_input.py' will generate needed input files for testing
Solution should output execution time in microseconds to stdout, and no other output.

## Deliverables
A PDF written report answering the taken approach as well as presenting the performance results and discussing any insights gained.
All source code including the (unmodified) makefile.
## Notes
Ensure implementations for parts 2 and 3 output a correct prefix sum for any number of threads.
Input sizes might not be a power of 2 and that case should be handled.
Do not print the length, or the number of elements when printing the output.
Ensure the given makefile is unmodified.

Report how much time was spent doing the lab.


## Parallel Work Implementation Discussion
Compute sum for neighbour
Compute sum of the resulting prefix neighbours now

## Work Efficient Parallel Scan

The up-sweep

The first thing to do is compute the prefix sum per even index of the first run
On subsequent runs the prefix sum is computed for the 2^depth as the offset in which to count the prefix's
This is done until the depth comes to an end.
To test that this portion works we know that the final value is indeed the prefix sum of all values, we can check and compare this value with the sequential final value to know if the prefix was outputted correctly.

The down-sweep
This part calculates the prefix scan portion, whereas the above gets us the prefix sum

