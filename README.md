# B-Trees-Array-Implementation
To build and experiment with B-Trees as array implementations.

Language Restriction: C

Implement a B tree with the following operations:
	
	1.	create
	
	2.	insert
	
	3.	delete
	
	4.	search.

Experiment with different values of t - the minimum degree of each node.

a) Implement the B-tree as an array.

	Each node shall have # of records(key: rest of the record).
	
	The number of keys (but for the root node) shall be between t - 1 and 2t - 1.
	
	Each node at a given point in time may have k keys and k + 1 references to its child trees.
	
	In this implementation, each reference will be an index of the array.

b) Implement the B-tree as a file.
	
	Each node shall be stored as a record in a file - the size of which should be equal
	to the block size for good performance.
	
	To locate a node in the file, you may use seek function(lseek or fseek) to change the offset
	in the file. Then you can read or write the whole node using block operation.
	read|write|fread|fwrite.

All operations shall be similar to the way they are in case of a tree implemented as an array.

Data-Set and Resources:	Dataset.

	
