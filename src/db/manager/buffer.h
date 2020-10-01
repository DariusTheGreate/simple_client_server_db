#ifndef  _BUFFER_
#define _BUFFER_
#include "block_list.h"
#include "file_list.h"

class buffer
{
private:
	file_list* fl;
	block_list* bl;
	std::string path;
	block* get_usable_block();
public:
	buffer(std::string p) : bl(new block_list(p)), fl(new file_list(p)), path(p) {}
	~buffer() {
		delete bl;
		delete fl;
	}

	block* get_file_block(std::string db_name, std::string tb_name, int file_type, int block_num);
	void write_block(block* block_in);
	void write_to_disk();
};



#endif // ! _BUFFER_
