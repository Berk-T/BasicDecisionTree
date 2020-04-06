/**
* Author : Berk Takit
* ID: 21803147
* Section : 2
* Assignment : 2
*/

#ifndef __DECISION_TREE
#define __DECISION_TREE
#include <string>
#include <fstream>
#include <iostream>
#include "DecisionFunctions.h"
#include "DecisionTreeNode.h"
using namespace std;
class DecisionTree {
public:
	DecisionTree();

	~DecisionTree();

	void train(const bool** data, const int* labels, const int numSamples, const
		int numFeatures);
	
	void train(const string fileName, const int numSamples, const int numFeatures);
	
	int predict(const bool* data);
	
	double test(const bool** data, const int* labels, const int numSamples);
	
	void print();

private:

	void buildTree(const bool** data, const int* labels, const int numSamples, const
		int numFeatures, bool*& usedSamples, bool*& testedFeatures, DecisionTreeNode* curNode);
	void createLeaf(DecisionTreeNode* node, const int numSamples, const bool* usedSamples, const int* labels);
	void createDecision(DecisionTreeNode* node, int featureIndex);
	double findBestSplit(const bool** data, const int* labels, const int numSamples, const
		int numFeatures, bool*& usedSamples, bool*& testedFeatures, int& featureIndex);
	void splitData(const bool** data, int numSamples, bool*& usedSamples, bool*& trueUsed, bool*& falseUsed, int featureIndex);
	void print(DecisionTreeNode* cur, int level);

	void deleteNode(DecisionTreeNode* cur);

	DecisionTreeNode* root;

};
#endif