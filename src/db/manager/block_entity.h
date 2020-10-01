#ifndef _BLOCK_
#define _BLOCK_
#include "file_entity.h"
#include <fstream>

class block
{
private:
	file *file_;
	int block_num;
	char* data;//change to class!
	bool dirty;
	long age;
	block *next;
public:
	block(int num) : dirty(false), next(nullptr), file_(nullptr), age(0), block_num(num) {
		data = new char[4 * 1024];
	}
	~block() { delete data; }

	file* get_file() { return file_; }
	void set_file(file* f) { file_ = f; }

	int get_block_num() { return block_num; }
	void set_block_num(int num) { block_num = num; }

	char* get_data() { return data; }
	void set_data(char* src) { data = src; }
	long get_age() { return age; }

	bool is_dirty() { return dirty; }
	void set_dirty(bool dt) { dirty = true; }

	block* get_next() { return next; }
	void set_next(block* block) { next = block; }

	void increasea_age() { ++age; }
	void reset_age() { age = 0; }

	void set_prev_block_num(int num) { *(int*)(data) = num; }

	int get_prev_block_num() { return *(int*)(data); }

	void set_next_block_num(int num) { *(int*)(data + 4) = num; }

	int get_next_block_num() { return *(int*)(data + 4); }

	void set_record_count(int count) { *(int*)(data + 8) = count; }

	void decrease_record_count() { *(int*)(data + 8) = *(int*)(data + 8) - 1; }

	int get_record_count() { return *(int*)(data + 8); }

	char* get_content_address() { return data + 12; }
	void read_info(std::string path);
	void write_info(std::string path);
};

#endif 