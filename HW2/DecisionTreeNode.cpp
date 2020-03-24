#include "DecisionTreeNode.h"

DecisionTreeNode::DecisionTreeNode() {
	isLeaf = false;
	nodeItem = NULL;
	left = NULL;
	right = NULL;
}

DecisionTreeNode::DecisionTreeNode(bool isLeaf, int nodeItem, DecisionTreeNode* left, DecisionTreeNode* right) {
	this->isLeaf = isLeaf;
	this->nodeItem = nodeItem;
	this->left = left;
	this->right = right;
}

DecisionTreeNode::~DecisionTreeNode() {
	nodeItem = NULL;
	left = NULL;
	right = NULL;
}