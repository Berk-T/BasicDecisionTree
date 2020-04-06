/**
* Author : Berk Takit
* ID: 21803147
* Section : 2
* Assignment : 2
*/


#include "DecisionFunctions.h"
#include <iostream>
#include <cmath>

// calculates the entropy for a given data set
double calculateEntropy(const int* classCounts, const int numClasses) {
	double entropy = 0.0;
	int totalClasses = 0;
	
	//Calculate total number of instances
	for (int i = 0; i < numClasses; i++) {
		totalClasses += classCounts[i];
	}

	for (int i = 0; i < numClasses; i++) {
		if (classCounts[i] != 0) {
			double weight = classCounts[i] / (double)totalClasses;
			entropy += weight * log2(weight);
		}
	}

	return -1.0 * entropy;
}

// infoGain = parent entropy - weighted average of children entropy
double calculateInformationGain(const bool** data, const int* labels,
	const int numSamples, const int numFeatures, const bool* usedSamples,
	const int featureId) {

	int noOfClasses = 0;
	int usedSize = 0;
	
	for (int i = 0; i < numSamples; i++) {

		//calculate number of different classes, not needed in this homework but offers
		//flexibility
		if (labels[i] > noOfClasses) noOfClasses = labels[i];

		//calculate how many samples are used for this node
		if (usedSamples[i]) usedSize++;
	}

	//initialize classCount for left right and parent
	int* left = new int[noOfClasses];
	int* right = new int[noOfClasses];
	int* parent = new int[noOfClasses];

	for (int i = 0; i < noOfClasses; i++) {
		left[i] = 0;
		right[i] = 0;
		parent[i] = 0;
	}

	int leftSize = 0; 
	int rightSize = 0;

	for (int i = 0; i < numSamples; i++) {
		if (usedSamples[i]) {
			parent[labels[i] - 1]++;
			//if sample at unused index i has that feature increase the class count for left
			if (data[i][featureId]) {
				left[labels[i] - 1]++;
				leftSize++;
			}//else increase class count for right
			else {
				right[labels[i] - 1]++;
				rightSize++;
			}
		}
	}

	double parentEntropy = calculateEntropy(parent, noOfClasses);
	double leftEntropy = calculateEntropy(left, noOfClasses);
	double rightEntropy = calculateEntropy(right, noOfClasses);

	double infoGain = parentEntropy - ((leftSize / (double)usedSize) * leftEntropy) - ((rightSize / (double)usedSize) * rightEntropy);

	delete[] left; 
	delete[] right;
	delete[] parent;

	return infoGain;
}