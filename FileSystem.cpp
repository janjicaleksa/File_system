#include "FileSystem.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>

FileSystem::FileSystem() {
	root_ = new TreeNode();
	stackRoots_.push(root_);
	b_ = false;
}

FileSystem::~FileSystem() {
	delete root_;
	root_ = nullptr;
	while (!stackRoots_.empty()) stackRoots_.pop();
}

void FileSystem::loadHierarchy(const string& fs_filepath, const string& log_filepath) {
	log_filepath_ = log_filepath;

	fstream input(fs_filepath, ios::in);

	string line = "";
	TreeNode* tmp;

		while (getline(input, line)) { // dokle god mozes da citas iz ulaznog fajla
			try {
				tmp = checkHierarchy(line); //proveri 
				root_->addChild(line, tmp); //i ubaci 
			}
			catch (FalsePath* e) {
				cout << e->what() << endl; //inace uhvati gresku
			}
		}
	
	input.close();
}

void FileSystem::execute(const string& filepath) {
	fstream input(filepath, ios::in);

	while (input.peek() != EOF) {
		string cmd;
		input >> cmd; //ucitaj poziv odgovarajuce komande 
		
		FileSystem::Command c = returnCommand(cmd);

		switch (c) {
			case LS: {
				commandLS();
				break;
			}
			case CD: {
				string strcd;
				input >> strcd;
				try {
					commandCD(strcd);
					break;
				}
				catch (FalseCommand* e) {
					string str = e->what();
					str.append("CD ");
					str.append(strcd);
					errors_.push_back(str);
					break;
				}
			}
			case NEW: {
				string strnew;
				getline(input,strnew); 
				try {
					commandNEW(strnew);
					break;
				}
				catch (FalseCommand* e) {
					string str = e->what();
					str.append("NEW");
					str.append(strnew);
					errors_.push_back(str);
					break;
				}
				/*string strnew = "";
				if (stackRoots_.top()->getParent() != nullptr) strnew = stackRoots_.top()->getNodePath();
				string strnew2;
				getline(input, strnew2);
				strnew2.replace(0, 1, 1, '\\');
				TreeNode* tmp = stackRoots_.top()->findChildByName(strnew2, stackRoots_.top());
				if (tmp == nullptr) {
					strnew.append(strnew2);
					commandNEW(strnew);
				}
				break;*/
			}
			case DEL: {
				string strdel;
				input >> strdel;
				try {
					commandDEL(strdel);
					break;
				}
				catch (FalseCommand* e) {
					string str = e->what();
					str.append("DEL ");
					str.append(strdel);
					errors_.push_back(str);
					break;
				}
			}
			case EXE: {
				string strexe;
				input >> strexe;
				try {
					commandEXE(strexe);
					break;
				}
				catch (FalseCommand* e) {
					string str = e->what();
					str.append("EXE ");
					str.append(strexe);
					errors_.push_back(str);
					break;
				}
			}
			case error: {
				break;
			}
		}
	}
	input.close();
}

TreeNode* FileSystem::checkHierarchy(string line) const {
	if (line[0] != '\\') { throw new FalsePath(); }; // ako prvi char linije nije kosa crta, vrati gresku
	int pos = 0;
	TreeNode* tmp = root_, *tmp2;
	while (line[pos] != '\0' && line[pos] != ' ') { //sve dok ne dodjes do kraja linije ili razmaka kod linija fajlova
		
		string str = readWord(line, pos); // citaj rec
		if (line[pos] == '\0') return tmp;
		
		tmp2 = tmp->findChildByName(str, tmp); // potrazi medju decom tu rec u nazivu fajla i dodeli narednom cvoru
		
		if (tmp2 != nullptr && str.find('.') != string::npos && line[pos] != '\0') { throw new FalsePath(); } //ako nije kraj putanje, a postoji fajl(sadrzi tacku), greska 

		if ((tmp2 == nullptr) && (line[pos] != ' ' && line[pos] != '\0')) throw new FalsePath(); //ako ne nadjes i ako nije kraj linije, baci gresku 
		else if ((tmp2 == nullptr) && (line[pos] == ' ' || line[pos] == '\0')) return tmp; //ako ne nadjes a jeste kraj linije, vrati tekuci cvor
			else tmp = tmp2; //inace pomeri se
	}
}

string FileSystem::readWord(string line, int& pos) const 
{ 
	if (line[pos] == '\\') pos++;//ako je kosa crta na pocetku, preskoci kosu crtu;
	string str;
	while (line[pos] != '\\' && line[pos] != '\0' && line[pos] != ' ') {
		str.push_back(line[pos]);
		pos++;
	}
	return str;
}

FileSystem::Command FileSystem::returnCommand(string cmd)
{
	if (cmd.compare("LS") == 0) return LS;
	else if (cmd.compare("CD") == 0) return CD;
	else if (cmd.compare("NEW") == 0) return NEW;
	else if (cmd.compare("DEL") == 0) return DEL;
	else if (cmd.compare("EXE") == 0) return EXE;
	else return error;
}

void FileSystem::commandLS() {
	root_ = root_->sortTree(root_); // prvo sortiraj celo stablo

	fstream output(log_filepath_, ios::app); // onda ispisi stablo 

	if (errors_.size() > 0) { //ako postoje greske, ispisi ih 
		for (int i = 0; i < errors_.size(); i++)
			output << errors_[i] << endl;
		errors_.clear(); //a onda izbrisi sve greske iz vektora
	}
	
	TreeNode* Root = stackRoots_.top();
	vector<string> vectorName;
	Root->printTree(output, Root, vectorName, b_); //ispis

	output.close();
}

void FileSystem::commandCD(string commandLine) {
	
	if (commandLine.compare("..") == 0) { //ako je u pitanju drugi nacin poziva komande CD
		TreeNode* newRoot = stackRoots_.top(); //uzmi sa vrha steka koren
		if (newRoot->getParent() == nullptr) throw new FalseCommand(); //ako je u pitanju pocetnji koren (njegov roditelj je nullptr), prijavices gresku
		else {
			stackRoots_.push(newRoot->getParent()); //u suprotnom nabaci na stek roditelja 
		}
	}
	else {//ako je u pitanju prvi nacin poziva komande 
		TreeNode* newRoot = stackRoots_.top()->findChildByName(commandLine, stackRoots_.top()); //potrazi medju decom
		
		if (newRoot != nullptr) stackRoots_.push(newRoot); //ako si pronasao, nabaci na stek
		else {
			throw new FalseCommand();
		}// u suprotnom greska
	}
}

void FileSystem::commandNEW(string command) {
	string str = "";
	if (stackRoots_.top()->getParent() != nullptr) str = stackRoots_.top()->getNodePath(); // dohvati putanju od korena

	command.replace(0, 1, 1, '\\'); //samo zameni prvi char koji je ' ' sa kosom crtom
	
	int i = 0;
	string name = readWord(command, i); //procitaj ime iz stringa komande

	TreeNode* tmp = stackRoots_.top()->findChildByName(name, stackRoots_.top()); //pronadji dete sa procitanim imenom

	if (tmp == nullptr) { //ako ne postoji dete sa tim imenom, dodaj
		str.append(command);
		stackRoots_.top()->addChild(str, stackRoots_.top());
	}
	else throw new FalseCommand(); //inace baci gresku 
}

void FileSystem::commandDEL(string command) {
	TreeNode* r = stackRoots_.top();
	TreeNode* tmp = r->findChildByName(command, r);
	
	if (tmp != nullptr) {
		//treba izbaciti podstablo iz dece korena i nabaciti na stek taj novi koren
		r->removeElementInVector(r, tmp);
		stackRoots_.push(r);
		//a onda obrisati nadjeno podstablo
		delete tmp;
	}
	else throw new FalseCommand();
}

void FileSystem::commandEXE(string command) {
	TreeNode* r = stackRoots_.top();
	TreeNode* tmp = r->findChildByName(command, r);
	if (tmp != nullptr) {
		string cont = tmp->getNodeContent();

		while (1) {
			int pos = cont.find('\\'); //pronadji gde se pojavljuje kosa crta

			if (pos == string::npos) //ako je nema, gotovo je sa preuredjivanjem teksta
				break;

			if (cont[pos + 1] == 'n') { //ako je naredni char 'n'
				cont.insert(pos, "\n"); //tamo gde si pronasao kosu crtu upisi '\n'
				cont.erase(pos + 1, 2); //a onda se pomeri jedno mesto unapred i obrisi 2 chara
			}
		}

		fstream inputExe("inputExe.txt", ios::out); //upisi obradjeni sadrzaj u pomocni fajl
		inputExe << cont;
		inputExe.close();

		execute("inputExe.txt"); //pozovi javnu funkciju za izvrsavanje svih komandi koje su upisane u pomocni fajl 
	}
	else throw new FalseCommand();
}