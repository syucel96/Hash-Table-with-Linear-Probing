#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <time.h>
using namespace std;

enum EntryType { ACTIVE, EMPTY, DELETED };

struct Entry
{
	int entry;
	EntryType info;
	Entry(const int &e = 0, const EntryType & information = EMPTY)
		:entry(e), info(information) {}
};
class Hashtable
{
public:
	Hashtable(const int &notFound, const int &tsize)
		:ITEM_NOT_FOUND(notFound), table(tsize), size(tsize) {
		makeEmpty();
		numEntries = 0;
		curProbes = 0;
	}
	Hashtable(const Hashtable &rhs)
		:ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),table(rhs.table),size(rhs.size){}
	int Hash(int o)
	{
		return (o % size);
	}
	int find(const int &x)
	{
		int current = Hash(x);
		curProbes = 0;
		int initial = current;
		if (table[current].info != EMPTY && table[current].entry != x)
		{
			current = Hash(current + 1);
			curProbes++;
		}
		//The search stops when the entry is found, when an empty node is reached or when it circulates through the whole array
		while (table[current].info != EMPTY && table[current].entry != x && current != initial)
		{
			current = Hash(current + 1);	//linear probing
			curProbes++;
		}
		if (isActive(current))
			return current;
		return ITEM_NOT_FOUND;
	}
	void makeEmpty()
	{
		for (int i = 0; i < size; i++)
			table[i].info = EMPTY;
		return;
	}
	bool isFull()
	{
		return size == numEntries;
	}
	int getProbes()
	{
		return curProbes;
	}
	int getNumEntries() const
	{
		return numEntries;
	}
	void insert(const int &x)
	{
		if (isFull())
			return;
		int current = Hash(x);
		curProbes = 0;
		//The search stops when the entry is found, when an empty or a deleted node is reached
		while (table[current].info != EMPTY && table[current].info != DELETED && table[current].entry != x)
		{
			current = Hash(current + 1);	//linear probing
			curProbes++;
		}
		if (x == table[current].entry)
			return;
		table[current] = Entry(x, ACTIVE);
		numEntries++;
		return;
	}
	void remove(const int &x)
	{
		int current = find(x);
		if (isActive(current))
		{
			table[current].info = DELETED;
			numEntries--;
		}
		return;
	}
private:
	int ITEM_NOT_FOUND;
	int size;
	vector <Entry> table;
	int numEntries;
	int curProbes;

	bool isActive(const int &pos) const
	{
		if (pos == ITEM_NOT_FOUND)
			return false;
		return table[pos].info == ACTIVE;
	}
};
bool isPrime(int& num)
{
	if (num == 2 || num == 3)
		return true;
	if ((num % 2 == 0) || (num == 1))
		return false;
	for (int i = 3; i ^ 2 <= num; i += 2)
	{
		if (num % i == 0)
			return false;
	}
	return true;
}
int nextPrime(int n)
{
	if (n % 2 == 0)
		n++;
	while (!isPrime(n))
		n += 2;
	return n;
}
void decide(unsigned int &i, unsigned int &d, unsigned int &f)//decide which transactions will take place
{
	int decider;
	i = 1;//all transaction counts are set to 1
	d = 1;
	f = 1;
	while ((i + d + f) < 8)
	{
		decider = rand() % 8;
		if (decider < i)
			i++;
		else if (decider < (i + d) && decider >= i)
			d++;
		else
			f++;
	}
	if (d >= i)//The number of inserts have to be larger than the number of deletes.
		decide(i, d, f);
	return;
}
int main()
{
	int M;
	//M = nextPrime(10000); nextPrime(10000) yields 10007, i commented this out because it took a long time
	M = 10007;
	int notfound = -1;
	Hashtable theTable = Hashtable(notfound, M);
	ofstream output1;
	output1.open("SuccessfulInserts.csv");
	ofstream output2;
	output2.open("FailedInserts.csv");
	ofstream output3;
	output3.open("SuccessfulDeletes.csv");
	ofstream output4;
	output4.open("FailedDeletes.csv");
	ofstream output5;
	output5.open("SuccessfulFinds.csv");
	ofstream output6;
	output6.open("FailedFinds.csv");
	unsigned int numTransactions = 0, i, d, f, e, iterator, prev,probes;
	int curr;
	cout << "Starting the transactions loop...\n";
	while ((!theTable.isFull()) && numTransactions < 1000000)//until the table is full or 1000000 entries are entered
	{
		decide(i, d, f);
		for (iterator = 0; iterator < i; iterator++)
		{
			e = rand() % (M * 10);
			prev = theTable.getNumEntries();
			theTable.insert(e);
			numTransactions++;
			curr = theTable.getNumEntries();
			probes = theTable.getProbes();
			if (prev == curr)//failed insert
				output2 << prev << ";" << probes << endl;
			else//successful insert
				output1 << prev << ";" << probes << endl;
		}
		if (theTable.isFull())
			break;
		for (iterator = 0; iterator < d; iterator++)
		{
			e = rand() % (M * 10);
			prev = theTable.getNumEntries();
			theTable.remove(e);
			numTransactions++;
			curr = theTable.getNumEntries();
			probes = theTable.getProbes();
			if (prev == curr)//failed delete
				output4 << prev << ";" << probes << endl;
			else//successful delete
				output3 << prev << ";" << probes << endl;
		}
		for (iterator = 0; iterator < f; iterator++)
		{
			e = rand() % (M * 10);
			prev = theTable.getNumEntries();
			curr = theTable.find(e);
			numTransactions++;
			probes = theTable.getProbes();
			if (-1 == curr)//failed find
				output6 << prev << ";" << probes << endl;
			else//successful find
				output5 << prev << ";" << probes << endl;
		}	
	}

	cout << "Total number of transactions = " << numTransactions << "\nTotal number of entries in the hash table = " << theTable.getNumEntries();
	if (numTransactions == 1000000)
		cout << "\nProgram terminated because the total number of transactions reached 1 000 000.\n ";
	else
		cout << "\nProgram terminated because the hash table is full.\n";
	output1.close();
	output2.close();
	output3.close();
	output4.close();
	output5.close();
	output6.close();
	system("pause");
	return 0;
}