/**
* Author : Berk Takit
* ID: 21803147
* Section : 2
* Assignment : 2
*/


#include "DecisionTreeNode.h"

// creates a new empty non-leaf node
DecisionTreeNode::DecisionTreeNode() {
	isLeaf = false;
	left = NULL;
	right = NULL;
}

// creates a node with the specified properties
DecisionTreeNode::DecisionTreeNode(bool isLeaf, int nodeItem, DecisionTreeNode* left, DecisionTreeNode* right) {
	this->isLeaf = isLeaf;
	this->nodeItem = nodeItem;
	this->left = left;
	this->right = right;
}

//deletes the node
DecisionTreeNode::~DecisionTreeNode() {
	left = NULL;
	right = NULL;
}