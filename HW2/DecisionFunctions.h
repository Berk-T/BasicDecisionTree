/**
* Author : Berk Takit
* ID: 21803147
* Section : 2
* Assignment : 2
*/


#ifndef __DECISION_FUNCTIONS
#define __DECISION_FUNCTIONS
using namespace std;

double calculateEntropy(const int* classCounts, const int numClasses);

double calculateInformationGain(const bool** data, const int* labels,
	const int numSamples, const int numFeatures, const bool* usedSamples,
	const int featureId);

#endif