#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
using namespace std;

class FalsePath : public exception {
public: 
	FalsePath() : exception("Error. False path.\n") {}
};

class FalseCommand : public exception {
public:
	FalseCommand() : exception("Error: ") {}
};

#endif 