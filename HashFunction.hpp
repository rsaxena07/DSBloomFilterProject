/*

	CS 261P - Data Structures Project 2
	Analysis of False Positive in 2 different BLoom Filters

	Title: Hash Function Implementation
	Authors: Rishabh Saxena | Chaitanya Prafull Ujeniya 
	UCInetID: rsaxena3 | cujeniya

*/
#ifndef INCLUDE_HASH_FUNCTION
#define INCLUDE_HASH_FUNCTION

/*	header file includes	*/
#include<climits>	// for INT_MAX
#include<cstdlib>	// for rand()

/*	Abstract Base class definition	*/
class HashFunction {

public:
	
	HashFunction() {
		//empty constructor - do nothing
	}

	virtual void initTable() = 0;	
	virtual int hashKey(int key) = 0;
};

/*	Tabular Hashing class implementation	*/
class TabulationHash : public HashFunction {
	/* Set of 4 table with 256 elements each */
	int TabulationTable[4][256];
public:
	/*	Default Constructor for the class. Calls the initilizer */
	TabulationHash():HashFunction() {
		initTable();
	}

	/*	Method to intitilize the table. Can be called anytime to re-hash the table */
	void initTable() {
		for (int i=0;i<4;i++) {
			for (int j=0;j<256;j++) {
				TabulationTable[i][j] = rand()%INT_MAX;
			}
		}
		return;
	}

	/*	Method that actually hashes a key and returns the value*/
	int hashKey(int key) {
		int h=0;
		for (int i=0;i<4;i++) {
			//extract 8 bits
			int lookup = key & ((1<<8)-1);
			// calculate hash value by XOR with table lookup
			h ^= TabulationTable[i][lookup];
			// shift next 8 bit to least significant position
			key = key>>8;
		}
		return h;
	}
};

#endif