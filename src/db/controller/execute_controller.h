#ifndef _EXECUTE_CONTROLLER_
#define _EXECUTE_CONTROLLER_
#include "../singletones/output_stream.h"
#include "../manager/catalog.h"
#include "../manager/buffer.h"
#include "execute_preparer.h"
#include <iostream>

class execute_controller
{
private:
	std::string path_;
	catalog* cm;
	std::vector<std::string> command;
	buffer* hdl;
protected:
	static std::string curr_db_;
	
public:
	std::string response;
	execute_controller(std::string p, std::vector<std::string> command_) : command(command_), path_(p), cm(new catalog(path_)), hdl(new buffer(path_)) { std::cout << "CONTROLLER CRETED" << std::endl; };
	void Quit();
	void Help();
	void CreateDatabase();
	void EmptyCommand();
	void ShowDatabases();
	void DropDatabase();
	void DropTable();
	void DropIndex();
	void Use();
	void CreateTable();
	void ShowTables();
	void Insert();
	void Select();
	void CreateIndex();
	void Delete();
	void Update();
};

#endif