#ifndef _FILE_TYPE_
#define _FILE_TYPE_
#include "standarts.h"
#include <string>

class block;

class file
{
private:
	std::string data_base;
	int file_type;
	std::string file_name;
	int record_len;
	int record_num;
	file * next;
	block * block_;
public:
	file() :  file_name(""), data_base(""), record_len(0), record_num(0), file_type(0), next(nullptr), block_(nullptr) {};
	file(std::string p, std::string name, std::string db, int len, int rn, int type) : file_name(name), data_base(db), record_len(len), record_num(0), file_type(type), next(nullptr), block_(nullptr) {};
	file(std::string db, int tp, std::string file_, int reca, int recl, file* nex, block* firb) : data_base(db), file_type(tp), file_name(file_), record_num(reca), record_len(recl), block_(firb), next(nex) {}

	std::string get_data_base() { return data_base; }

	int get_file_type() { return file_type; }

	std::string get_file_name() { return file_name; }

	block* get_first_block() { return block_; }

	void set_first_block(block* bp) { block_ = bp; }

	file* get_next() { return next; }
	void set_next(file* fp) { next = fp; }

	void IncreaseRecordAmount() { record_num++; }
	void IncreaseRecordLength() { record_len += 4096; }
};

#endif 
