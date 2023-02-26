#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "TreeNode.h"

class FileSystem {
public: 
	
	FileSystem();
	~FileSystem();
	void loadHierarchy(const string& fs_filepath, const string& log_filepath);
	void execute(const string& filepath);
	enum Command { LS, CD, NEW, DEL, EXE, error };

private:
	string log_filepath_;
	TreeNode* root_;
	stack<TreeNode*> stackRoots_;
	vector<string> errors_;
	bool b_;

	TreeNode* checkHierarchy(string line) const;
	string readWord(string line, int& pos) const;

	Command returnCommand(string cmd);
	void commandLS();
	void commandCD(string command);
	void commandNEW(string command);
	void commandDEL(string command);
	void commandEXE(string command);
};
#endif