#ifndef _EXECUTE_PREPARER_
#define _EXECUTE_PREPARER_
#include <vector>
#include <iostream>
#include <string>
#include "../includes/exceptions.h"
#include "../manager/catalog.h"
#include "../singletones/output_stream.h"
#include <boost/algorithm/string.hpp>
class attribute;


class value {
public:

	int data_type;
	std::string value_;
	value() : data_type(-1), value_("") {};
	value(int& t, std::string v) : data_type(t), value_(v) {};
};

class execute_preparer
{
public:
	int parse_data_type(std::vector<std::string> sql_vector, attribute& attr, unsigned int pos);
};

class execute_database_creator : public execute_preparer
{
private:
	std::string db_name_;
public:
	execute_database_creator(const std::vector<std::string>&& command);
	std::string get_db_name() { return db_name_; }
};

class execute_database_droper : public execute_preparer
{
private:
	std::string db_name;

public:
	execute_database_droper(const std::vector<std::string>&& command);
	std::string get_db_name() { return db_name; }
};

class execute_database_user : public execute_preparer
{
private:
	std::string db_name;

public:
	execute_database_user(const std::vector<std::string>&& command);
	std::string get_db_name() { return db_name; }
};

class execute_database_tablecreator : public execute_preparer
{
private:
	std::vector<attribute> _attrs_;
	std::string tb_name;
public:
	execute_database_tablecreator(const std::vector<std::string>&& command);
	std::vector<attribute> get_attrs() { return _attrs_; };
	std::string get_tb_name() { return tb_name; }
};


class execute_table_droper : public execute_preparer
{
private:
	std::string tb_name;

public:
	execute_table_droper(const std::vector<std::string>&& command);
	std::string get_tb_name() { return tb_name; }
};

class execute_insert_preparer : execute_preparer
{
private:
	std::string tb_name_;
	std::vector<value> values_;

public:
	execute_insert_preparer(const std::vector<std::string>&& sql_vector);
	void parse(std::vector<std::string> sql_vector);
	std::string tb_name() { return tb_name_; }
	std::vector<value>& values() { return values_; }
};

#endif