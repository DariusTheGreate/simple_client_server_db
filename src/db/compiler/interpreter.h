#ifndef MINIDB_INTERPRETER_H_
#define MINIDB_INTERPRETER_H_

#include <string>
#include <vector>
#include <iostream>
#include "../controller/execute_controller.h"
#include "parser.h"

class Interpreter {
private:
	execute_controller* executer;
	parser *parser_;
public:
	Interpreter(parser* p);
	~Interpreter();
	void Run();
	std::string get_response();
};

#endif 