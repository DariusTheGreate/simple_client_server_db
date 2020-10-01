#ifndef _PARSER_
#include <iostream>
#include <vector>


#define MIN(x, y) ((x) < (y) ? (x) : (y)) 
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "../includes/exceptions.h"
#include "../singletones/output_stream.h"
struct command_info
{
	command_info(int a, std::string b) : sql_type(a), command(b) {};
	command_info() : sql_type(0), command("") {};

	int sql_type;
	std::string command;
	void print() { std::cout << "start server" << std::endl; }
};

class parser
{
private:
	std::string sql_statement_;
	std::string path_location;
	std::vector<std::string> sql_vector_;
	std::vector<command_info> command_types;
	int sql_type_;
	void TellSQLType();
	void FormatSQL();
	command_info get_prediction(std::string input);
public:
	std::string get_path() { return path_location; }
	std::vector<std::string> get_command() { return sql_vector_; }
	void change_command(const std::string&& coma) { sql_statement_ = coma; };
	void parse();
	int get_state() const { return sql_type_; };
	parser(std::string command);
	~parser();
};

#define _PARSER_
#endif