/*

	CS 261P - Data Structures Project 2
	Analysis of False Positive in 2 different BLoom Filters

	Title: Hash Function Implementation
	Authors: Rishabh Saxena | Chaitanya Prafull Ujeniya 

*/
#ifndef INCLUDE_BLOOM_FILTER
#define INCLUDE_BLOOM_FILTER

/*	Include header files*/
#include<vector>
#include<iostream>

#include "HashFunction.hpp"

using namespace std;

class AbstractBloomFilter {

public:
	AbstractBloomFilter() {
		// do nothing
	}

	virtual void insertKey(int key) = 0;
	virtual bool testMembership(int key) = 0;
};


class BloomFilter : public AbstractBloomFilter {

	const int numberOfHashFunctions;
	const int filterTableLength;

	vector<HashFunction*> hashFunctions;
	vector<bool> bloomFilterHashTable;		// Table of bool

	// This ensures that the returned values of any internal hash function are modded to the hashTableLength
	int hashKey(int hashFunctionIdx, int key) {
		return (hashFunctions[hashFunctionIdx]->hashKey(key) % filterTableLength);
	}

public:
	
	/*	Default Constructor for Bloom Filter to set parameters and allocate bloomFilterHashTable vector*/
	BloomFilter():numberOfHashFunctions(3), filterTableLength(30) {
		
		bloomFilterHashTable = vector<bool>(30, false);

		for (int i=0;i<3;i++) {
			hashFunctions.push_back(new TabulationHash());	
		}
	}

	/*	Parametrized Constructor for Bloom Filter to set parameters, allocate bloomFilterHashTable vector, and assign HashFunctions*/
	BloomFilter(vector<HashFunction*> newHashFunctions, int k, int N): numberOfHashFunctions(k), filterTableLength(N) {
		// Allocate & Initialize the table to false
		bloomFilterHashTable = vector<bool>(N, false);
		
		// sanity check .. we dont initialize a new Hash Function in constructor as we want both bloom filters to have the same hash functions
		if (newHashFunctions.size()!=k) {
			cout<<"Bloom Filter Creation Possible Error";
		}

		// assign HashFunctions
		hashFunctions = newHashFunctions;
	}

	int getK() {
		return numberOfHashFunctions;
	}

	int getN() {
		return filterTableLength;
	}
 	
 	/*	Override: Method to insertKey to bloomFilter 
 		Returns void		*/
 	void insertKey(int key) {
		for (int i=0;i<numberOfHashFunctions;i++) {
			bloomFilterHashTable[this->hashKey(i, key)] = true;
		}
 	}

 	/*	Override: Method to insertKey to bloomFilter 
 		Returns false is key is definitely not in the Set
 		Return true if the key is maybe in the Set. (Either present or false positive)		*/
 	bool testMembership(int key) {
		for (int i=0;i<numberOfHashFunctions;i++) {
 			if (!bloomFilterHashTable[this->hashKey(i, key)]) {
 				return false;
 			}
 		}
 		return true;
 	}

 	void toString() {

 		cout<<"Regular Bloom Filter"<<endl;
 		for (int i=0;i<filterTableLength;i++) {
 			cout<<"["<<i<<"]"<<" ";
 		}
 		cout<<endl;
 		for (int i=0;i<filterTableLength;i++) {
 			int space = i/10;
 			while (space>0) {
				cout<<" ";
				space/=10;
			}
 			cout<<"["<<bloomFilterHashTable[i]<<"]"<<" ";
 		}
 		cout<<endl;
 	}

 	vector<int> getHashValues (int key) {
 		vector<int> hashedIdx;
 		for (int i=0;i<numberOfHashFunctions;i++) {
 			hashedIdx.push_back(this->hashKey(i, key));
 		}
 		return hashedIdx;
 	}

};

class KArrayBloomFilter : public AbstractBloomFilter {

	const int numberOfHashFunctions;
	const int filterTableLength;

	vector<HashFunction*> hashFunctions;
	vector<vector<bool> > bloomFilterHashTable;		// Table of bool

	// This ensures that the returned values of any internal hash function are modded to the hashTableLength
	int hashKey(int hashFunctionIdx, int key) {
		return (hashFunctions[hashFunctionIdx]->hashKey(key) % filterTableLength);
	}

public:

	/*	Default Constructor*/
	KArrayBloomFilter():numberOfHashFunctions(3), filterTableLength(10) {

		vector<bool> bloomFilterHashTableRow = vector<bool>(10, false);
		for (int i=0;i<3;i++) {
			bloomFilterHashTable.push_back(bloomFilterHashTableRow);
			hashFunctions.push_back(new TabulationHash());	
		}
	}

	/*	Parametrized Constructor for Bloom Filter to set parameters, allocate bloomFilterHashTable vector, and assign HashFunctions*/
	KArrayBloomFilter(vector<HashFunction*> newHashFunctions, int k, int N): numberOfHashFunctions(k), filterTableLength(N) {
		// Allocate & Initialize the table to false
		vector<bool> bloomFilterHashTableRow(N, false);
		
		for (int i=0;i<k;i++) {
			bloomFilterHashTable.push_back(bloomFilterHashTableRow);
		}

		// sanity check .. we dont initialize a new Hash Function in constructor as we want both bloom filters to have the same hash functions
		if (newHashFunctions.size()!=k) {
			cout<<"Bloom Filter Creation Possible Error";
		}

		// assign HashFunctions
		hashFunctions = newHashFunctions;
	}

	int getK() {
		return numberOfHashFunctions;
	}

	int getN() {
		return filterTableLength;
	}

	/*	Override: Method to insertKey to KArray bloomFilter 
 		Returns void		*/
 	void insertKey(int key) {
		for (int i=0;i<numberOfHashFunctions;i++) {
			bloomFilterHashTable[i][this->hashKey(i, key)] = true;
		}
 	}

 	/*	Override: Method to insertKey to KArray bloomFilter 
 		Returns false is key is definitely not in the Set
 		Return true if the key is maybe in the Set. (Either present or false positive)		*/
 	bool testMembership(int key) {
		for (int i=0;i<numberOfHashFunctions;i++) {
 			if (!bloomFilterHashTable[i][this->hashKey(i, key)]) {
 				return false;
 			}
 		}
 		return true;
 	}

 	void toString() {

 		cout<<"K-Array Bloom Filter"<<endl;
 		
 		for (int j=0;j<numberOfHashFunctions;j++) {
 			cout<<"Row "<<j<<endl;
 			for (int i=0;i<filterTableLength;i++) {
 			cout<<"["<<i<<"]"<<" ";
	 		}
	 		cout<<endl;
	 		for (int i=0;i<filterTableLength;i++) {
	 			int space = i/10;
	 			while (space>0) {
	 				cout<<" ";
	 				space/=10;
	 			}
	 			cout<<"["<<bloomFilterHashTable[j][i]<<"]"<<" ";
	 		}
	 		cout<<endl<<endl;
 		}
 	}

	vector<int> getHashValues (int key) {
 		vector<int> hashedIdx;
 		for (int i=0;i<numberOfHashFunctions;i++) {
 			hashedIdx.push_back(this->hashKey(i, key));
 		}
 		return hashedIdx;
 	}
};


#endif
