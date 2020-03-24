#ifndef __DECISION_TREE_NODE
#define __DECISION_TREE_NODE
#include <cstddef>
using namespace std;
class DecisionTreeNode {
private:

	DecisionTreeNode();
	DecisionTreeNode(bool isLeaf, int nodeItem, DecisionTreeNode* left = NULL, DecisionTreeNode* right = NULL );
	~DecisionTreeNode();

	int nodeItem; //represents class if isLeaf, feature to test otherwise
	DecisionTreeNode* left;
	DecisionTreeNode* right;
	bool isLeaf;

	friend class DecisionTree;
};

#endif