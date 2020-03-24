#include "DecisionTree.h"
#include "DecisionFunctions.h"

DecisionTree::DecisionTree() {
	root = new DecisionTreeNode;
	testedFeatures = NULL;
}

DecisionTree::~DecisionTree() {
	if (root != NULL) {
		deleteNode(root);
	}

	delete[] testedFeatures;
}

void DecisionTree::deleteNode(DecisionTreeNode* cur) {
	if (cur == NULL) return;

	DecisionTreeNode* tmpLeft = cur->left;
	DecisionTreeNode* tmpRight = cur->right;
	delete cur;

	deleteNode(tmpLeft);
	deleteNode(tmpRight);
}


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

void DecisionTree::train(const bool** data, const int* labels, const int numSamples, const
	int numFeatures) {

	bool* usedSamples = new bool[numSamples];
	testedFeatures = new bool[numFeatures];

	for (int i = 0; i < numFeatures; i++) {
		testedFeatures[i] = false;
	}

	for (int i = 0; i < numSamples; i++) {
		usedSamples[i] = true;
	}

	buildTree((const bool**)data, labels, numSamples, numFeatures, usedSamples, root);

	cout << "finished training" << endl;

	delete[] usedSamples;

}


//true left false right
void DecisionTree::buildTree(const bool** data, const int* labels, const int numSamples, const
	int numFeatures, bool*& usedSamples, DecisionTreeNode* curNode) {

	int featureIndex;
	double gain = findBestSplit(data, labels, numSamples, numFeatures, usedSamples, featureIndex);

	if (featureIndex >= 0) testedFeatures[featureIndex] = true;

	if (gain == 0.0) {
		createLeaf(curNode, numSamples, usedSamples, labels);
		return;
	}
	
	createDecision(curNode, featureIndex);

	bool* trueUsed = NULL;
	bool* falseUsed = NULL;

	splitData(data, numSamples, usedSamples, trueUsed, falseUsed, featureIndex);

	//build true branch
	DecisionTreeNode* trueBranch = new DecisionTreeNode;
	curNode->left = trueBranch;
	buildTree(data, labels, numSamples, numFeatures, trueUsed, trueBranch);

	//build false branch
	DecisionTreeNode* falseBranch = new DecisionTreeNode;
	curNode->right = falseBranch;
	buildTree(data, labels, numSamples, numFeatures, falseUsed, falseBranch);

	delete[] trueUsed;
	delete[] falseUsed;
	return;
}

double DecisionTree::findBestSplit(const bool** data, const int* labels, const int numSamples, const
	int numFeatures, bool*& usedSamples, int& featureIndex) {

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

void DecisionTree::createDecision(DecisionTreeNode* node, int featureIndex) {
	if (node == root) {
	}
	node->isLeaf = false;
	node->nodeItem = featureIndex;
}

int DecisionTree::predict(const bool* data) {
	DecisionTreeNode* curNode = root;
	while (!curNode->isLeaf) {
		if (data[curNode->nodeItem]) {
			curNode = curNode->left;
		}
		else {
			curNode = curNode->right;
		}
	}

	return curNode->nodeItem;
}

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

void DecisionTree::print() {
	print(root, 0);
}

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