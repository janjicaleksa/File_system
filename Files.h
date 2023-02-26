#ifndef FILES_H
#define FILES_H
#include <string>
using namespace std;

class File {
public:
	enum Extension { txt, exe, folder };
	File(string line);
	string getLine() const;
	string getName() const;
	string getPath() const;
	string getContent() const;
private:
	string line_;
	string path_;
	string name_;
	Extension extension_;
	string content_;

	void setInfo();
};
#endif