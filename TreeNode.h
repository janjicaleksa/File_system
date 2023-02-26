#ifndef TREENODE_H
#define TREENODE_H

#include "Files.h"
#include <fstream>
#include <stack>
#include <vector>


class TreeNode {
public:
	TreeNode();
	TreeNode(string line, TreeNode* parent);
	//TreeNode(const TreeNode& node);
	//TreeNode(TreeNode&& node);
	~TreeNode();

	void addChild(string line, TreeNode* parent);
	string getNodeName() const;
	string getNodePath() const;
	string getNodeContent() const;
	TreeNode* getParent() const;
	TreeNode* findChildByName(string str, TreeNode* tmp);
	void removeElementInVector(TreeNode* root, TreeNode* tmp);

	TreeNode* sortTree(TreeNode* root);
	void printTree(ostream& output, TreeNode* root, vector<string>& vectorName, bool& b);
	
private:
	File* file_;
	TreeNode* parent_;
	vector<TreeNode*> children_;

	int stringCompare(const string& s1, const string& s2);
};
#endif