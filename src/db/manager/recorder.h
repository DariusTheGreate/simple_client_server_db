
#ifndef _RECORDER_
#define _RECORDER_

#include <string>
#include <vector>

#include "block_entity.h"
#include "buffer.h"
#include "catalog.h"
#include "exceptions.h"
#include "../controller/execute_preparer.h"
#include "key.h"

class recorder {
private:
	buffer* hdl_;
	catalog* cm_;
	std::string db_name_;

public:
	recorder(catalog* cm, buffer* hdl, std::string db) : cm_(cm), hdl_(hdl), db_name_(db) {}
	~recorder() {}
	void insert(execute_insert_preparer& prep);
	//void select(select_preparer& prep);
	//void delete_record(delete_preparer& prep);
	//void update(update_preparer& prep);

	block* get_block(table* tbl, int block_num);
	std::vector<key> get_record(table* tbl, int block_num, int offset);
	void DeleteRecord(table* tbl, int block_num, int offset);
	void UpdateRecord(table* tbl, int block_num, int offset, std::vector<int>& indices, std::vector<key>& values);

	//bool SatisfyWhere(table* tbl, std::vector<key> keys, SQ where);
};

#endif /* _RECORDER_ */
