/*

	CS 261P - Data Structures Project 2
	Analysis of False Positive in 2 different BLoom Filters

	Title: Main Tests
	Authors: Rishabh Saxena | Chaitanya Prafull Ujeniya 
	UCInetID: rsaxena3 | cujeniya

*/

#include <cstdlib>	// for stdio
#include <fstream>
#include <set>


#include "HashFunction.hpp"
#include "BloomFilter.hpp"

using namespace std;

#define GENERAL_BLOOM_FILTER_ROW_SIZE 5000
#define NUM_FALSE_POSITIVE_TEST_KEYS_ABS 10000 
#define NUM_FALSE_POSITIVE_TEST_KEYS_REL_SCALE 100
#define RANDOM_KEY_LIMIT 1000000

#define MIN_NUM_INSERTIONS 50
#define MAX_NUM_INSERTIONS 1000
#define NUM_INSERTIONS_INCR 25
#define MIN_NUM_HASH_FUNCTIONS 2
#define MAX_NUM_HASH_FUNCTIONS 10
#define NUM_HASH_FUNCTIONS_INCR 1

// Main Test Driver Code 
int main() {

	srand(10);	// random seed

	ofstream generatedKeysDump, testingDataDump;
	generatedKeysDump.open("data/generatedKeysDump.csv");
	testingDataDump.open("data/BloomFilterTestData.csv");

	// Iterating through a range of number of keys to insert and number of hash functions to test on
	for(int n=MIN_NUM_INSERTIONS; n<=MAX_NUM_INSERTIONS; n+=NUM_INSERTIONS_INCR) {
		for (int h=MIN_NUM_HASH_FUNCTIONS; h<=MAX_NUM_HASH_FUNCTIONS; h+=NUM_HASH_FUNCTIONS_INCR) {
			
			set<int> inserted_keys; 	//Set to store the inserted values to be used for sanity check
			vector<HashFunction*> hashFunctions;
			//Loop to generate the hash functions and store in the vector
			for(int j=0;j<h;j++)
			{
				hashFunctions.push_back(new TabulationHash());
			}

			BloomFilter filter_1(hashFunctions,h,(GENERAL_BLOOM_FILTER_ROW_SIZE));
			KArrayBloomFilter filter_2(hashFunctions,h,GENERAL_BLOOM_FILTER_ROW_SIZE/h);

			int k=0;
			while (k<n) {
				int random_num=rand()%RANDOM_KEY_LIMIT;
				if (inserted_keys.find(random_num) == inserted_keys.end()) {
					//only add a key to bloom filter if it has not been inserted before
					inserted_keys.insert(random_num);
					filter_1.insertKey(random_num);
					filter_2.insertKey(random_num);	
					if (k!=0)
						generatedKeysDump<<",";
					generatedKeysDump<<random_num;
					k++;
				}
			}
			generatedKeysDump<<endl;

			// sanity check
			for (set<int>::iterator it = inserted_keys.begin(); it != inserted_keys.end(); it++) {
				int insertedKey = *it;
				if (!filter_1.testMembership(insertedKey)) {
					cout<<"Regular Bloom Filter Insertion ERROR: Inserted key "<<insertedKey<<" not found";
				}
				if (!filter_2.testMembership(insertedKey)) {
					cout<<"K Array Bloom Filter Insertion ERROR: Inserted key "<<insertedKey<<" not found";
				}
			}

			//false positive test
			int t = NUM_FALSE_POSITIVE_TEST_KEYS_ABS;
			int true_positives_general = 0, false_positive_general = 0;
			int true_positives_k_array = 0, false_positive_k_array = 0;
			for (int l=0;l<t;l++) {
				int random_num=rand()%RANDOM_KEY_LIMIT;
				if (filter_1.testMembership(random_num)) {
					if (inserted_keys.find(random_num) != inserted_keys.end()) {
						// key found in bloom filter and inserted key set
						true_positives_general++;
					} else {
						// false positive scenario
						false_positive_general++;
					}
				}

				if (filter_2.testMembership(random_num)) {
					if (inserted_keys.find(random_num) != inserted_keys.end()) {
						// key found in bloom filter and inserted key set
						true_positives_k_array++;
					} else {
						// false positive scenario
						false_positive_k_array++;
					}
				}

				if (l!=0)
					generatedKeysDump<<",";
				generatedKeysDump<<random_num;
			}
			generatedKeysDump<<endl;

			double general_FPR = ((double) false_positive_general * 100.0 / t);
			double k_array_FPR = ((double) false_positive_k_array * 100.0 / t);
		
			cout<<" k = "<<h<<" num insertions = "<<n<<" false positive keys generated = "<<t<<endl;
			cout<<"False Positive Rate for General Bloom Filter : "<<general_FPR<<" "<<false_positive_k_array<<endl;
			cout<<"False Positive Rate for K Array Bloom Filter : "<<k_array_FPR<<" "<<false_positive_general<<endl<<endl;
		
			// dumping data to data collection csv
			testingDataDump<<h<<","<<n<<","<<t<<","<<true_positives_general<<","<<false_positive_general<<",";
			testingDataDump<<true_positives_k_array<<","<<false_positive_k_array<<","<<general_FPR<<","<<k_array_FPR<<endl;
		
		}
	}

	testingDataDump.close();
	generatedKeysDump.close();

	return 0;
}