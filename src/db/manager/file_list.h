#ifndef _FILE_LIST_
#define _FILE_LIST_
#include "file_entity.h"
#include "block_entity.h"
#include <iostream>

class file_list
{
private:
	file* head;
	std::string path;
public:
	file_list(std::string p) : path(p), head(new file()) {}
	~file_list();
	file* get_file(std::string db_name, std::string tb_name, int file_type);
	block* get_block(file* file, int block_pos);
	void add_block(block*);
	void increase_age();
	block* recycle_block();
	void add_file(file* file_in);
	void write_to_disk();
	void read_from_disk();
};
#endif 
