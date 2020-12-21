
## Info:

Feel free to use these lines as you wish.
Local search algorithms to compute graph cuts.

## To compile:

"make"

## To execute:

"./bisection edgelist.txt res.txt [bisection]".

"edgelist.txt" should contain an undirected unweighted graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.

"res.txt" will contain the resulting bisection: one node ID followed by 0 or 1 on each line.

option "bisection": the algorithm to use, default is 1.
- 0: random bisection
- 1: greedy sparsest cut
- 2: greedy densest cut
- 3: greedy max cut
- 4: greedy min cut

## Initial contributor:
Maximilien Danisch  
April 2019  
http://bit.ly/danisch  
maximilien.danisch@gmail.com
