# Distributed Systems HomeWork 2
## MPI Programming

### Problem 1

   - We are first augmenting the matrix such that matrix ``A -> [A|I]`.
   - We then rearrange the rows in the descending order of the 1st element of each row. We are doing this so that we can avoid the division by zero in the next step.
   - Now use the 'Scatter' to distribute each row to each process.
   - Now we do row transformation to make the non diagonal element 0. Row(x) = Row(x)- (λ)*Row(y)
   - Now we divide the Row `a[i]` by `a[i][rank]` so as to make the diagonal element 1.
   - Now we use 'Gather' to collect all the rows of the augmented matrix.
   - Now the augmented matrix is INV(a).

   - Imp: The number of rows should be less than or equal to the number of number processors as each processor handles only one row. 

	
---

### Problem 3

- We note that it is sufficient to calculate the sum upto 100 terms as after that they are very neglible and won't contribute much in the sum. 
- To each process we send a range of integers(dividing equally) to process there result and send it back. The root process then sums up all of the terms. 
- For calculating one term we just calculate the factorial of `2*rank+1` and take divide it by `pow(x,rank)`. The sign is determined by the parity of rank. 
- For calculating the partial sum which consists of sum of many terms we sum them in a single loop updating the numerator and dinominator. 
- To avoid overflow issues with factorial of large number, we instead update the term by dividing by new denominator and new numerator. i.e  ` value *= ((long double)(x) / i);` 

### Problem 6
   - We want a valid vertex coloring of the graph in no more than delta + 1 colors, where delta is the maximum of the degrees of all the vertices in the graph. 

    - We start by assigning random weights to each vertex in the graph.
    - In each iteration, we find all those vertices that are the maximum amongst their uncolored neighbors. They will form an independent set.
    - Since they form an independent set, we can color them parallely without any contradiction.
    - We color the node with the least possible color value such that none of the neighbour is colored in the same color.
    - To parallelize this even further, the root process will not find the independent set. Instead, that task is divided with each process where each process finds an independent set from the nodes in it's range.
    - Repeat this until all nodes are colored.
