#include "TreeNode.h"
#include <iostream>

TreeNode::TreeNode() : file_(nullptr), parent_ (nullptr) {}

TreeNode::TreeNode(string line, TreeNode* parent) {
	
	file_ = new File(line);
	parent_ = parent; 
	
}

/*TreeNode::TreeNode(const TreeNode& node) {
	file_ = new File(node.file_->getLine());
	parent_ = node.parent_;
	for (int i = 0; i < node.children_.size(); i++) {
		children_.push_back(node.children_[i]);
		node.children_[i] = nullptr;
	}
}*/

TreeNode::~TreeNode() {
	if (children_.size() > 0) {
		for (int i = 0; i < children_.size(); i++) {
			delete children_[i]; 
		}
	}
	delete file_;
	file_ = nullptr;
	parent_ = nullptr;
	
}

/*TreeNode::TreeNode(TreeNode&& node) {
	file_ = node.file_;
	parent_ = node.parent_;
	for (int i = 0; i<node.children_.size(); i++) {
		children_.push_back(node.children_[i]);
		node.children_[i] = nullptr;
	}
	node.file_ = nullptr;
	node.parent_ = nullptr;
}*/

void TreeNode::addChild(string line, TreeNode* parent) {
	
	TreeNode* newNode = new TreeNode(line, parent);
	parent->children_.push_back(newNode);
}

string TreeNode::getNodeName() const {
	return file_->getName();
}

string TreeNode::getNodePath() const {
	return file_->getPath();
}

string TreeNode::getNodeContent() const
{
	return file_->getContent();
}

TreeNode* TreeNode::getParent() const {
	return parent_;
}

TreeNode* TreeNode::findChildByName(string str, TreeNode* tmp) {
	for (auto it : tmp->children_)
	{
		string name = it->getNodeName();
		int c = name.compare(str);
		if (c == 0) {
			return it;
			break;
		}
	}
	return nullptr;
}

void TreeNode::removeElementInVector(TreeNode* root, TreeNode* tmp) {
	for (size_t i = 0; i < root->children_.size(); i++) { //prodji kroz sve elemente vektora
		if (tmp->getNodeName().compare(root->children_[i]->getNodeName()) == 0) //ako su isti nazivi 
			root->children_.erase(root->children_.begin() + i); //obrisi na toj poziciji
	}
}

TreeNode* TreeNode::sortTree(TreeNode* root) {
	TreeNode* tmp = root; 
	//sortiraj prvo decu korena
	for (size_t i = 0; i < tmp->children_.size() - 1; i++) {
		for (size_t j = i + 1; j < tmp->children_.size(); j++) {
			if (stringCompare(tmp->children_[i]->getNodeName(), tmp->children_[j]->getNodeName()) == -1)
				swap(tmp->children_[i], tmp->children_[j]);
		}
	}
	
	//onda prodji kroz svako dete korena i ispitaj da li ima dalje dece, ako ima, neka se sortira rekurzivno
		
	for (size_t i = 0; i < tmp->children_.size(); i++) {
		TreeNode* tmp2 = tmp->children_[i];
		if (tmp2->children_.size() > 0) tmp2 = tmp2->sortTree(tmp2);
	}

	return tmp;
}

void TreeNode::printTree(ostream& output, TreeNode* root, vector<string>& vectorName, bool& b) {

	for (size_t i = 0; i < root->children_.size(); i++) {
		
		if (b == true) output << endl;
		else b = true;

		TreeNode* tmp = root->children_[i]; //dohvati dete korena

		if (vectorName.size() > 0) {//ako postoji bilo sta u vektoru imena 
			if (tmp->getParent()->getNodeName().compare(vectorName.back()) != 0) vectorName.clear(); //ako se roditeljski naziv ne nalazi na kraju vektora imena, obrisi sve u vektoru
			else for (auto it : vectorName) output << it; // u suprotnom ispisi sve iz vektora
		}

		output << "\\" << tmp->getNodeName(); //ispisi modifikovani naziv cvora
				
		if (tmp->children_.size() > 0) { //ako ima dalje dece taj cvor
			vectorName.push_back("\\");
			vectorName.push_back(tmp->getNodeName()); //dodaj njegovo ime na kraj vektora
			printTree(output, tmp, vectorName, b); //ispisi rekurzivno njegovu decu
		}
	}
}

int TreeNode::stringCompare(const string& s1, const string& s2) {

	int n = s1.length() > s2.length() ? s2.length() : s1.length(); //uzima se duzina kraceg stringa
	for (int i = 0; i < n; i++) {
		if (s1[i] == '_' && s2[i] != '_') return 1; //prioritet ima donja crta
		else if (s1[i] != '_' && s2[i] == '_') return -1;
		if (isupper(s1[i]) && islower(s2[i]) && (islower(s2[i]) - isupper(s1[i])) == 32) continue; // u ASCII tabeli vecu vrednost imaju mala slova od velikih, a u nasem zadatku su istog prioriteta 
		if (isupper(s2[i]) && islower(s1[i]) && (islower(s1[i]) - isupper(s2[i])) == 32) continue; 

		if (s1[i] < s2[i]) return 1; //standardno ispitivanje char po char 
		else if (s1[i] > s2[i]) return -1;
		else continue;
	}
	return 0;
}