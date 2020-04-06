/**
* Author : Berk Takit
* ID: 21803147
* Section : 2
* Assignment : 2
*/

#include "DecisionTree.h"
#include "DecisionFunctions.h"

// creates an empty decisiontree and initializes the root node
DecisionTree::DecisionTree() {
	root = new DecisionTreeNode;
}

// deletes the tree starting from the root
DecisionTree::~DecisionTree() {
	if (root != NULL) {
		deleteNode(root);
	}
}

// deletes the specified node
void DecisionTree::deleteNode(DecisionTreeNode* cur) {
	if (cur == NULL) return;

	DecisionTreeNode* tmpLeft = cur->left;
	DecisionTreeNode* tmpRight = cur->right;
	delete cur;

	deleteNode(tmpLeft);
	deleteNode(tmpRight);
}

// initializes the training data by parsing the given data set text file
void DecisionTree::train(const string fileName, const int numSamples, const int numFeatures) {
	ifstream inFile;
	inFile.open(fileName);

	if (!inFile.is_open()) {
		cout << "Unable to open file " << fileName << endl;
		return;
	}

	bool** data = new bool*[numSamples];
	int* labels = new int[numSamples];

	for (int i = 0; i < numSamples; i++) {
		data[i] = new bool[numFeatures];
		for (int j = 0; j < numFeatures; j++) {
			inFile >> data[i][j];
		}
		inFile >> labels[i];
	}

	inFile.close();

	train((const bool**)data, labels, numSamples, numFeatures);

	for (int i = 0; i < numSamples; i++) {
		delete[] data[i];
	}
	delete[] data;

	delete[] labels;
	return;
}

// starts the tree building process
void DecisionTree::train(const bool** data, const int* labels, const int numSamples, const
	int numFeatures) {

	bool* usedSamples = new bool[numSamples];
	bool* testedFeatures = new bool[numFeatures];

	for (int i = 0; i < numFeatures; i++) {
		testedFeatures[i] = false;
	}

	for (int i = 0; i < numSamples; i++) {
		usedSamples[i] = true;
	}

	buildTree((const bool**)data, labels, numSamples, numFeatures, usedSamples, testedFeatures, root);

	delete[] usedSamples;
	delete[] testedFeatures;

}

//recursively builds the tree by selecting the best feature to split by and splitting
//the data accordingly. Creates a leaf node if there are no more decisions that are beneficial.
//left branch is false & right branch is true
void DecisionTree::buildTree(const bool** data, const int* labels, const int numSamples, const
	int numFeatures, bool*& usedSamples, bool*& testedFeatures, DecisionTreeNode* curNode) {

	int featureIndex;
	double gain = findBestSplit(data, labels, numSamples, numFeatures, usedSamples, testedFeatures, featureIndex);

	if (featureIndex >= 0) testedFeatures[featureIndex] = true;

	if (gain == 0.0) {
		createLeaf(curNode, numSamples, usedSamples, labels);
		return;
	}
	
	createDecision(curNode, featureIndex);

	bool* trueUsed = NULL;
	bool* falseUsed = NULL;

	splitData(data, numSamples, usedSamples, trueUsed, falseUsed, featureIndex);


	bool* leftTested = new bool[numFeatures];
	bool* rightTested = new bool[numFeatures];
	for (int i = 0; i < numFeatures; i++) {
		leftTested[i] = testedFeatures[i];
		rightTested[i] = testedFeatures[i];
	}

	//build true branch
	DecisionTreeNode* trueBranch = new DecisionTreeNode;
	curNode->right = trueBranch;
	buildTree(data, labels, numSamples, numFeatures, trueUsed, rightTested, trueBranch);

	//build false branch
	DecisionTreeNode* falseBranch = new DecisionTreeNode;
	curNode->left = falseBranch;
	buildTree(data, labels, numSamples, numFeatures, falseUsed, leftTested, falseBranch);

	delete[] leftTested;
	delete[] rightTested;
	delete[] trueUsed;
	delete[] falseUsed;
	return;
}

//Finds the best feature to split the data by calculating information gain.
double DecisionTree::findBestSplit(const bool** data, const int* labels, const int numSamples, const
	int numFeatures, bool*& usedSamples, bool*& testedFeatures, int& featureIndex) {

	double bestGain = 0;
	int bestFeatureIndex = -1;

	for (int i = 0; i < numFeatures; i++) {
		if (!testedFeatures[i]) {
			double gain = calculateInformationGain(data, labels, numSamples, numFeatures, usedSamples, i);

			if (gain > bestGain) {
				bestGain = gain;
				bestFeatureIndex = i;
			}
		}
	}

	featureIndex = bestFeatureIndex;
	return bestGain;
}

//Splits the data into two sets based on the feature
void DecisionTree::splitData(const bool** data, int numSamples, bool*& usedSamples, 
	bool*& trueUsed, bool*& falseUsed, int featureIndex) {
	
	//copy existing used to both branches
	trueUsed = new bool[numSamples]; 
	falseUsed = new bool[numSamples];
	for (int i = 0; i < numSamples; i++) {
		trueUsed[i] = false;
		falseUsed[i] = false;
	}

	for (int i = 0; i < numSamples; i++) {
		if (usedSamples[i]) {
			if (data[i][featureIndex]) {
				trueUsed[i] = true;
			}
			else {
				falseUsed[i] = true;
			}
		}
	}

	return;
}

//Creates a leaf node that assigns a class
void DecisionTree::createLeaf(DecisionTreeNode* node, const int numSamples, const bool* usedSamples, const int* labels) {
	
	node->isLeaf = true;

	int noOfClasses = 0;

	//calculate number of different classes
	for (int i = 0; i < numSamples; i++) {
		if (labels[i] > noOfClasses) noOfClasses = labels[i];
	}

	int* classCount = new int[noOfClasses];

	for (int i = 0; i < noOfClasses; i++) {
		classCount[i] = 0;
	}

	for (int i = 0; i < numSamples; i++) {
		if (usedSamples[i]) {
			classCount[labels[i] - 1]++;
		}
	}

	int classNo = -1;
	int curMaxClassAmount = 0;

	for (int i = 0; i < noOfClasses; i++) {
		if (classCount[i] > curMaxClassAmount) {
			curMaxClassAmount = classCount[i];
			classNo = i + 1;
		}
	}

	node->nodeItem = classNo;

	node->left = NULL;
	node->right = NULL;

	delete[]classCount;
}

//Creates a decision node
void DecisionTree::createDecision(DecisionTreeNode* node, int featureIndex) {
	if (node == root) {
	}
	node->isLeaf = false;
	node->nodeItem = featureIndex;
}

//predicts the class of a given sample using the trained decision tree
int DecisionTree::predict(const bool* data) {
	DecisionTreeNode* curNode = root;
	while (!curNode->isLeaf) {
		if (data[curNode->nodeItem]) {
			curNode = curNode->right;
		}
		else {
			curNode = curNode->left;
		}
	}

	return curNode->nodeItem;
}

//Tests how accurate the decision tree is by testing all samples in a test set
double DecisionTree::test(const bool** data, const int* labels, const int numSamples) {
	int rightPredictions = 0;
	this->print();
	for (int i = 0; i < numSamples; i++) {
		if (labels[i] == predict(data[i])) {
			rightPredictions++;
		}
	}

	return rightPredictions / (double)numSamples;
}

//Starts the recursive printing function
void DecisionTree::print() {
	cout << "Decision Tree:" << endl;
	print(root, 0);
}

//Recursively prints the decision tree by preorder traversal, tabs represent levels
void DecisionTree::print(DecisionTreeNode* cur, int level) {

	if (cur == NULL) return;

	for (int i = 0; i < level; i++) cout << "\t";

	if (cur->isLeaf) cout << "class=" << cur->nodeItem << endl;
	else cout << cur->nodeItem << endl;
	level++;

	print(cur->left, level);
	print(cur->right, level);
}


int main() {
	DecisionTree t;

	t.train("train_data.txt", 498, 21);
	
	string fileName = "test_data.txt";
	int numSamples = 473;
	int numFeatures = 21;
	ifstream inFile;
	inFile.open(fileName);

	if (!inFile.is_open()) {
		cout << "Unable to open file " << fileName << endl;
		return -1;
	}

	bool** data = new bool* [numSamples];
	int* labels = new int[numSamples];

	for (int i = 0; i < numSamples; i++) {
		data[i] = new bool[numFeatures];
		for (int j = 0; j < numFeatures; j++) {
			inFile >> data[i][j];
		}
		inFile >> labels[i];
	}

	inFile.close();

	cout << "Is right " << (t.test((const bool**)data, labels, numSamples) * 100.0) << "% of the time" << endl;

	for (int i = 0; i < numSamples; i++) {
		delete[] data[i];
	}
	delete[] data;

	delete[] labels;
	return 0;
}