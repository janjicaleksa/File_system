#include "Files.h"
#include <iostream>

File::File(string line) {
	line_ = line;
	setInfo();
}

string File::getLine() const {
	return line_;
}

string File::getName() const {
	return name_;
}

string File::getPath() const {
	return path_;
}

string File::getContent() const {
	return content_;
}

void File::setInfo() {
	//podesavanje putanje foldera/fajla
	int i = 0;
	while (line_[i] != ' ' && line_[i] != '\0') {
		path_ += line_[i]; 
		i++;
	}
	//podesavanje imena foldera/fajla
	i = 0;
	while (line_[i] != ' ' && line_[i] != '\0') i++;
	while (line_[i] != '\\') i--;
	++i; // preskace kosu crtu 
	while (line_[i] != ' ' && line_[i] != '\0') {
		name_ += line_[i];
		i++;
	}
	//podesavanje ekstenzije 
	int pos = line_.find('.');
	if (pos == string::npos) {
		extension_ = folder;
		content_ = "";
		return;
	}; //ako nema nigde tacke, znaci da su u pitanju sve folderi i onda nema svrhe menjati ekstenziju i sadrzaj
	string str = "";
	pos++; // preskace tu nadjenu tacku
	while (line_[pos] != ' ') { 
		str += line_[pos]; 
		pos++;
	}
	if (str == "txt") extension_ = txt;
	if (str == "exe") extension_ = exe;
	//podesavanje sadrzaja
	pos = line_.find(' ');
	pos++;
	while (line_[pos] != '\0') {
		content_ += line_[pos];
		pos++;
	}
}