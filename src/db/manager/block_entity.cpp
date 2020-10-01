#include "block_entity.h"

void block::read_info(std::string path)
{
	path += (file_)->get_data_base() + "/" + file_->get_file_name();
	if (file_->get_file_type() == FORMAT_INDEX) {
		path += ".index";
	}
	else {
		path += ".records";
	}

	std::ifstream ifs(path, std::ios::binary);
	ifs.seekg(block_num * 4 * 1024);
	ifs.read(data, 4 * 1024);
	ifs.close();
}

void block::write_info(std::string path) {
	path += file_->get_data_base() + "/" + file_->get_file_name();
	if (file_->get_file_type() == FORMAT_INDEX) {
		path += ".index";
	}
	else {
		path += ".records";
	}

	std::ofstream ofs(path, std::ios::binary);
	ofs.seekp(block_num * 4 * 1024);
	ofs.write(data, 4 * 1024);
	ofs.close();
}