This C++11 code of Knuth Algorith X with Doubly Linked Lists, called [DLX][dlx].

It prints out all solutions to [Pentomino][pent] puzzle for a NxM rectangular board.

In each solution, board tiles are numbered 0 to 59.

Output example:

	0 1 7 13 14 Z
	2 8 9 15 16 W
	3 4 5 10 11 P
	6 12 18 19 24 Y
	17 21 22 23 29 T
	20 25 26 27 32 X
	28 34 35 41 47 N
	30 36 42 43 44 V
	31 33 37 38 39 U
	40 45 46 52 53 F
	48 49 50 51 54 L
	55 56 57 58 59 I

	0 6 7 8 14 Z
	1 2 3 4 5 I
	9 10 11 15 17 U
	12 13 18 19 24 P
	16 21 22 23 29 F
	20 25 26 27 32 X
	28 33 34 38 39 W
	30 36 42 48 49 L
	31 37 43 44 45 V
	35 40 41 47 53 Y
	46 52 57 58 59 T
	50 51 54 55 56 N

	0 6 7 8 14 Z
	1 2 3 4 9 Y
	5 10 11 15 16 W
	12 13 18 19 24 P
	17 23 29 35 41 I
	20 25 26 27 32 X
	21 22 28 33 34 U
	30 31 37 38 39 N
	36 42 48 54 55 L
	40 44 45 46 52 T
	43 49 50 51 56 F
	47 53 57 58 59 V

	0 6 7 8 14 Z
	1 2 3 4 9 Y
	5 10 11 15 16 W
	12 13 18 19 24 P
	17 21 22 23 29 T
	20 25 26 27 32 X
	28 34 35 41 47 N
	30 36 42 43 44 V
	31 33 37 38 39 U
	40 45 46 52 53 F
	48 49 50 51 54 L
	55 56 57 58 59 I

	Found 2339 solutions
	Matrix generation time: 0.0122784 seconds
	Elapsed time: 27.5229 seconds

[pent]: http://en.wikipedia.org/wiki/Pentomino
[dlx]: http://en.wikipedia.org/wiki/Dancing_Links
