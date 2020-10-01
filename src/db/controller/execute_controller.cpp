#include "execute_controller.h"
#include <boost/filesystem.hpp>
#include "../manager/recorder.h"
#include <fstream>
#include <iostream>

std::string execute_controller::curr_db_ = "";

void execute_controller::Help() {
	output_stream* out = output_stream::GetInstance("\n");
	out->append_to_value(out -> value());
	out	-> set_value_to(std::string("MiniDB 1.0.0\n"));
	out->append_to_value(std::string("help to get help"));
}

void execute_controller::Quit()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("im quiitin"));
	delete cm;
}

void execute_controller::EmptyCommand()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its not a command"));
}

void execute_controller::CreateDatabase()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("create database control\n"));

	execute_database_creator* creator = new execute_database_creator(std::move(command));
	
	std::string folder_name(path_ + '/' + creator -> get_db_name());
	
	out->append_to_value(std::move(folder_name));
	out->append_to_value(std::move(folder_name));

	boost::filesystem::path folder_path(folder_name);

	folder_path.imbue(std::locale("en_US.UTF-8"));

	if (cm->get_db(creator -> get_db_name()) != NULL) {
		throw DatabaseAlreadyExistsException();
	}

	if (boost::filesystem::exists(folder_path)) {
		boost::filesystem::remove_all(folder_path);
		out->append_to_value("Database folder exists and deleted\n!");
	}

	boost::filesystem::create_directories(folder_path);
	out->append_to_value("Database folder created!\n");
	cm->create_database(creator -> get_db_name());

	out->append_to_value("Catalog written!");
	cm->write_archive_file();
}

void execute_controller::ShowDatabases()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("database list:\n"));
	
	std::vector<database> dbs = cm->dbs();
	for (unsigned int i = 0; i < dbs.size(); ++i) {
		database db = dbs[i];
		out->append_to_value("\t");
		out->append_to_value(std::move(db.get_db_name()));
		out->append_to_value("\n");
	}
}

void execute_controller::DropDatabase()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("Dropping database: "));
	execute_database_droper* droper = new execute_database_droper(std::move(command));
	out->append_to_value(std::move(droper->get_db_name()));
	out->append_to_value("\n");
	bool found = false;

	std::vector<database> dbs = cm->dbs();
	for (unsigned int i = 0; i < dbs.size(); ++i) {
		if (dbs[i].get_db_name() == droper->get_db_name()) {
			found = true;
		}
	}

	if (found == false) {
		throw DatabaseNotExistException();
	}

	std::string folder_name(path_ + droper->get_db_name());
	boost::filesystem::path folder_path(folder_name);

	folder_path.imbue(std::locale("en_US.UTF-8"));

	if (!boost::filesystem::exists(folder_path)) {
		out->append_to_value("Database folder doesn't exists!\n");
	}
	else {
		boost::filesystem::remove_all(folder_path);
		out->append_to_value("Database folder deleted!\n");
	}

	cm->delete_database(droper->get_db_name());
	out->append_to_value("Database removed from catalog!\n");
	cm->write_archive_file();

	if (droper -> get_db_name() == curr_db_) {
		curr_db_ = "";
		delete hdl;
	}
}

void execute_controller::Use()
{

	std::cout << "here " << curr_db_ << std::endl;
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("use database "));
	execute_database_user* user = new execute_database_user(std::move(command));

	database* db = cm->get_db(user->get_db_name());
	out->append_to_value(std::move(user->get_db_name()));
	out->append_to_value("\n");
	if (db == NULL) {
		throw DatabaseNotExistException();
	}

	if (curr_db_.length() != 0) {
		out->append_to_value("Closing the old database: \n");
		cm->write_archive_file();
		delete hdl;
	}
	std::string db_temp_name = user->get_db_name();
	curr_db_ = db_temp_name;
	hdl = new buffer(path_);

	std::cout << "here " << curr_db_ << std::endl;
}

void execute_controller::CreateTable()
{
	std::cout << "here " << curr_db_ << std::endl;
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("table creating\n"));
	execute_database_tablecreator creator(std::move(command));
	out->append_to_value(std::move(curr_db_));
	out->append_to_value("\n");
	if (curr_db_.length() == 0) {
		throw NoDatabaseSelectedException();
	}

	database* db = cm->get_db(curr_db_);
	if (db == NULL) {
		throw DatabaseNotExistException();
	}

	if (db->get_table(creator.get_tb_name()) != NULL) {
		throw TableAlreadyExistsException();
	}
	std::string file_name(path_ + "/" + curr_db_ + "/" + creator.get_tb_name() + ".records");

	std::cout << "FILE naME IS " << file_name << std::endl;
	boost::filesystem::path folder_path(file_name);

	if (boost::filesystem::exists(file_name)) {
		boost::filesystem::remove(file_name);
		out->append_to_value("Table file already exists and deleted!\n");
	}

	std::ofstream ofs(file_name);
	ofs.close();
	out->append_to_value("Table file created!\n");

	db->create_table(creator);
	out->append_to_value("Catalog written!\n");
	cm->write_archive_file();
}

void execute_controller::DropTable()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("droping\n"));
	execute_table_droper dropper(std::move(command));
	if (curr_db_.length() == 0) {
		throw NoDatabaseSelectedException();
	}

	database* db = cm->get_db(curr_db_);
	if (db == NULL) {
		throw DatabaseNotExistException();
	}

	table* tb = cm->get_db(curr_db_)->get_table(dropper.get_tb_name());

	if (tb == NULL) {
		throw TableNotExistException();
	}

	std::string file_name(path_ + "/" + curr_db_ + "/" + dropper.get_tb_name() + ".records");

	if (!boost::filesystem::exists(file_name)) {
		std::cout << "Table file doesn't exist!" << std::endl;
	}
	else {
		boost::filesystem::remove(file_name);
		std::cout << "Table file removed!" << std::endl;
	}

	std::cout << "Removing Index files!" << std::endl;
	out->append_to_value("Removing Index files!\n");
	for (int i = 0; i < tb->get_index_num(); ++i) {
		std::string file_name(path_ + curr_db_ + "/" + tb->get_index(i)->name() + ".index");
		if (!boost::filesystem::exists(file_name)) {
			std::cout << "Index file doesn't exist!" << std::endl;
			out->append_to_value("Index file doesn't exist!\n");
		}
		else {
			boost::filesystem::remove(file_name);
			std::cout << "Index file removed!" << std::endl;
			out->append_to_value("Index file removed!\n");
		}
	}
	
	db->drop_table(dropper);
	std::cout << "Catalog written!" << std::endl;
	out->append_to_value("Catalog written!\n");
	cm->write_archive_file();
}

void execute_controller::DropIndex()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its not a command"));
}

void execute_controller::ShowTables()
{
	if (curr_db_.length() == 0) {
		throw NoDatabaseSelectedException();
	}
	output_stream* out = output_stream::GetInstance("\n");

	database* db = cm->get_db(curr_db_);
	if (db == NULL) {
		throw DatabaseNotExistException();
	}
	
	out->set_value_to(std::string("current database is: "));
	out->append_to_value(std::move(curr_db_));

	std::cout << "TABLE LIST:" << std::endl;
	out->append_to_value("\ntables list is: \n");

	for (int i = 0; i < db->tbs().size(); ++i) {
		table tb = db->tbs()[i];
		std::cout << "\t" << tb.get_tb_name() << std::endl;
		out->append_to_value("\t");
		out->append_to_value(tb.get_tb_name());
		out->append_to_value("\n");
	}
}

void execute_controller::Insert()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("insert..."));
	execute_insert_preparer prep(std::move(command));
	recorder *rec = new recorder(cm,hdl,curr_db_);
	rec -> insert(prep);
	delete rec;
}

void execute_controller::Select()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its not a command"));
}

void execute_controller::CreateIndex()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its not a command"));
}
void execute_controller::Delete()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its not a command"));
}
void execute_controller::Update()
{
	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its not a command"));
}