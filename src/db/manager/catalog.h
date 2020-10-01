#ifndef _CATALOG_
#define _CATALOG_

#include <string>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "../controller/execute_preparer.h"

class database;
class table;
class index;
class attribute;
class execute_database_tablecreator;
class execute_table_droper;
class catalog
{
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& databases;
	}
	std::string path;
	std::vector<database> databases;
public:
	catalog(std::string p);
	~catalog();
	std::vector<database>& dbs() { return databases; }
	std::string get_path() { return path; }
	database* get_db(std::string db_name);
	void read_archive_file();
	void write_archive_file();
	void create_database(std::string dbname);
	void delete_database(std::string dbname);
};

class database
{
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& db_name_;
		ar& tables;
	}
	std::string db_name_;
	std::vector<table> tables;
public:
	database() {}
	database(std::string dbname);
	~database() {}
	table* get_table(std::string tb_name);
	std::string get_db_name() { return db_name_; }
	void create_table(execute_database_tablecreator& creator);
	void drop_table(execute_table_droper& dropper);
	//void DropIndex(SQLDropIndex& st);
	std::vector<table>& tbs() { return tables; }
	bool check_if_index_exists(std::string index_name);
};

class table
{
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& tb_name;
		ar& record_length;
		ar& first_block_num;
		ar& first_rubbish_num;
		ar& block_count;
		ar& attributes;
		//ar& ids_;
	}

	std::string tb_name;
	int record_length;

	int first_block_num;
	int first_rubbish_num;
	int block_count;
	std::vector<attribute> attributes;
	std::vector<index> ids_;

public:
	std::string get_tb_name() { return tb_name; }
	void add_attr(const attribute&& attr) { attributes.push_back(attr); };

	table() : tb_name(""), record_length(-1), first_block_num(-1), first_rubbish_num(-1), block_count(0) {}
	~table() {}

	void set_tb_name(std::string tbname) { tb_name = tbname; }

	int get_record_length() { return record_length; }
	void set_record_length(int len) { record_length = len; }

	std::vector<attribute>& ats() { return attributes; }
	attribute* get_attribute(std::string name);
	int get_attribute_index(std::string name);

	int get_first_block_num() { return first_block_num; }
	void set_first_block_num(int num) { first_block_num = num; }
	int get_first_rubbish_num() { return first_rubbish_num; }
	void set_first_rubbish_num(int num) { first_rubbish_num = num; }
	int get_block_count() { return block_count; }

	unsigned long get_attribute_num() { return attributes.size(); }
	void AddAttribute(attribute& attr) { attributes.push_back(attr); }
	void IncreaseBlockCount() { block_count++; }

	std::vector<index>& get_ids() { return ids_; }
	index* get_index(int num) { return &(ids_[num]); }
	unsigned long get_index_num() { return ids_.size(); }
	void AddIndex(index& idx) { ids_.push_back(idx); }
};

class attribute
{
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& attr_name_;
		ar& data_type_;
		ar& length_;
		ar& attr_type_;
	}

	std::string attr_name_;
	int data_type_;
	int length_;
	int attr_type_;

public:
	attribute() : attr_name_(""), data_type_(-1), length_(-1), attr_type_(0) {}
	~attribute() {}

	std::string attr_name() { return attr_name_; }

	void set_attr_name(std::string name) { attr_name_ = name; }

	int attr_type() { return attr_type_; }
	void set_attr_type(int type) { attr_type_ = type; }

	int data_type() { return data_type_; }
	void set_data_type(int type) { data_type_ = type; }

	void set_length(int length) { length_ = length; }
	int length() { return length_; }
};


class index {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& max_count_;
		ar& attr_name_;
		ar& name_;
		ar& key_len_;
		ar& key_type_;
		ar& rank_;
		ar& rubbish_;
		ar& root_;
		ar& leaf_head_;
		ar& key_count_;
		ar& level_;
		ar& node_count_;
	}
	int max_count_;
	int key_len_;
	int key_type_;
	int rank_;
	int rubbish_;
	int root_;
	int leaf_head_;
	int key_count_;
	int level_;
	int node_count_;
	std::string attr_name_;
	std::string name_;

public:
	index() {}
	index(std::string name, std::string attr_name, int keytype, int keylen,
		int rank) {
		attr_name_ = attr_name;
		name_ = name;
		key_count_ = 0;
		level_ = -1;
		node_count_ = 0;
		root_ = -1;
		leaf_head_ = -1;
		key_type_ = keytype;
		key_len_ = keylen;
		rank_ = rank;
		rubbish_ = -1;
		max_count_ = 0;
	}

	// accessors and mutators
	std::string attr_name() { return attr_name_; }

	int key_len() { return key_len_; }

	int key_type() { return key_type_; }

	int rank() { return rank_; }

	int root() { return root_; }
	void set_root(int root) { root_ = root; }

	int leaf_head() { return leaf_head_; }
	void set_leaf_head(int leaf_head) { leaf_head_ = leaf_head; }

	int key_count() { return key_count_; }
	void set_key_count(int key_count) { key_count_ = key_count; }

	int level() { return level_; }
	void set_level(int level) { level_ = level; }

	int node_count() { return node_count_; }
	void set_node_count(int node_count) { node_count_ = node_count; }

	std::string name() { return name_; }

	int IncreaseMaxCount() { return max_count_++; }
	int IncreaseKeyCount() { return key_count_++; }
	int IncreaseNodeCount() { return node_count_++; }
	int IncreaseLevel() { return level_++; }
	int DecreaseKeyCount() { return key_count_--; }
	int DecreaseNodeCount() { return node_count_--; }
	int DecreaseLevel() { return level_--; }
};

#endif