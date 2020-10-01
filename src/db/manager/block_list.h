#ifndef _BLOCK_LIST_
#define _BLOCK_LIST_
#include "block_entity.h"

class block_list
{
private:
	block* head;
	int length;
	int count;
	std::string path;
	block* init(block* b);
public:
	block_list(std::string p);
	~block_list();
	int bcount();
	block* get_usable_block();
	void free_block(block*);
};

#endif
