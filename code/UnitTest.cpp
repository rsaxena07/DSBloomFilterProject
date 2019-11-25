/*

	CS 261P - Data Structures Project 2
	Analysis of False Positive in 2 different BLoom Filters

	Title: Unit Tests
	Authors: Rishabh Saxena | Chaitanya Prafull Ujeniya 

*/

#include<vector>
#include<iostream>
#include<cstdlib>		// for rand()

#include "BloomFilter.hpp"

int main() {


	srand(10);
	
	//declare the obects of the two bloomfilters
	BloomFilter filter_1;
	KArrayBloomFilter filter_2;

	vector<int> inputKeys1;
	vector<int> inputKeys2;
	inputKeys1.push_back(10);
	inputKeys1.push_back(20);
	inputKeys1.push_back(30);
	inputKeys1.push_back(40);
	inputKeys1.push_back(50);
	inputKeys2.push_back(60);
	inputKeys2.push_back(80);
	inputKeys2.push_back(90);
	inputKeys2.push_back(70);
	inputKeys2.push_back(100);
	
	
	//insert 

	for (int i=0;i<inputKeys1.size();i++) {
		
		filter_1.insertKey(inputKeys1[i]);
		vector<int> hashIdx = filter_1.getHashValues(inputKeys1[i]);

		cout<<"KeyIndex: "<<inputKeys1[i]<<" Table Entries Set: "<<hashIdx[0]<<" "<<hashIdx[1]<<" "<<hashIdx[2]<<endl;
	}

	cout<<endl<<endl;
	filter_1.toString();
	

	cout<<endl;
	vector<int> bucket1, bucket2, bucket3;
	for (int i=0;i<inputKeys2.size();i++) {
		filter_2.insertKey(inputKeys2[i]);

		vector<int> hashIdx = filter_2.getHashValues(inputKeys2[i]);
		cout<<"KeyIndex: "<<inputKeys2[i]<<" Table Entries Set: "<<hashIdx[0]<<" "<<hashIdx[1]<<" "<<hashIdx[2]<<endl;

		bucket1.push_back(hashIdx[0]);
		bucket2.push_back(hashIdx[1]);
		bucket3.push_back(hashIdx[2]);
	}

	cout<<endl<<"Row 0 set bits ";
	for (int i=0;i<bucket1.size();i++) {
		cout<<bucket1[i]<<" ";
	}
	cout<<endl<<"Row 1 set bits ";
	for (int i=0;i<bucket1.size();i++) {
		cout<<bucket2[i]<<" ";
	}
	cout<<endl<<"Row 2 set bits ";
	for (int i=0;i<bucket1.size();i++) {
		cout<<bucket3[i]<<" ";
	}

	cout<<endl<<endl;
	filter_2.toString();

	// test membership

	bool success = true;
	for (int i=0;i<inputKeys1.size();i++) {
		if (!filter_1.testMembership(inputKeys1[i])) {
			success = false;
		}
	}
	if (!success) {
		cout<<" Regular BloomFilter Test membership	Test FAILURE \n";
	} else {
		cout<<" Regular BloomFilter Test membership	Test SUCCESS \n";
	}

	success = true;
	for (int i=0;i<inputKeys2.size();i++) {
		if (!filter_2.testMembership(inputKeys2[i])) {
			success = false;
		}
	}
	if (!success) {
		cout<<" K-Array BloomFilter Test membership	Test FAILURE \n";
	} else {
		cout<<" K-Array BloomFilter Test membership	Test SUCCESS \n";
	}


}
