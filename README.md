# Hash-Table-with-Linear-Probing
implements a hash table and fills it to determine how number of elements affect load factor

This program implements a hash table of size 10007 and fills it with random inputs using linear probing. There are 3 possible operations that can be performed on the hashtable:Insert, Delete and Find.
The types of operations to be performed are determined randomly. Insert must be the first operation and there must be at least one insert operation in every 8 operations. 
The program runs until the hashtable is full and the following .csv files containing the number of entries prior to the operation and the number of probes needed for that operation are created:
"SuccessfulInserts.csv", "FailedInserts.csv", "SuccessfulDeletes.csv", "FailedDeletes.csv", "SuccessfulFinds.csv", "FailedFinds.csv"

These .csv files are then utilized to draw graphs for Number of Probes vs Load Factor. The graphs and further explanation for a sample run can be seen in the PDF file.
