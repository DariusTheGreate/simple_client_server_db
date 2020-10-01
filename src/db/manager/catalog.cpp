#include "catalog.h"


#include <boost/filesystem.hpp>

using namespace std;

//=======================catalog=======================//

catalog::catalog(std::string p) : path(p) { read_archive_file(); }

catalog::~catalog() { write_archive_file(); }

void catalog::read_archive_file() {
    std::string file_name = path + "catalog";
    boost::filesystem::path file_path(file_name);

    file_path.imbue(std::locale("en_US.UTF-8"));

    if (boost::filesystem::exists(file_path)) {
        std::ifstream ifs;
        ifs.open(file_name.c_str(), std::ios::binary);
        boost::archive::binary_iarchive iar(ifs);
        iar >> (*this);
        ifs.close();
    }
}

void catalog::write_archive_file() {
    std::string file_name = path + "catalog";
    std::cout << "CATALO WRITE\n";
    std::ofstream ofs;
    ofs.open(file_name.c_str(), std::ios::binary);
    boost::archive::binary_oarchive oar(ofs);
    oar << (*this);
    ofs.close();
}

void catalog::create_database(std::string dbname) {
    databases.push_back(database(dbname));
}

void catalog::delete_database(std::string dbname) {
    for (unsigned int i = 0; i < databases.size(); i++) {
        if (databases[i].get_db_name() == dbname) {
            databases.erase(databases.begin() + i);
        }
    }
}

database *catalog::get_db(std::string db_name) {
    for (unsigned int i = 0; i < databases.size(); ++i) {
        if (databases[i].get_db_name() == db_name) {
            return &databases[i];
        }
    }
    return NULL;
}


database::database(std::string dbname) : db_name_(dbname) {}

void database::create_table(execute_database_tablecreator& creator) {
    int record_length = 0;
    table tb;
    for (int i = 0; i < creator.get_attrs().size(); ++i) {
        tb.add_attr(std::move(creator.get_attrs()[i]));
        record_length += creator.get_attrs()[i].length();
    }

    tb.set_tb_name(creator.get_tb_name());
    tb.set_record_length(record_length);
    tables.push_back(tb);
}

void database::drop_table(execute_table_droper& dropper) {
    vector<table>::iterator i;
    for (i = tables.begin(); i != tables.end(); i++) {
        if (i->get_tb_name() == dropper.get_tb_name()) {
            tables.erase(i);
            return;
        }
    }
}

table* database::get_table(std::string tb_name) {
    for (unsigned int i = 0; i < tables.size(); ++i) {
        if (tables[i].get_tb_name() == tb_name) {
            return &tables[i];
        }
    }
    return NULL;
}

/*bool database::check_if_index_exists(std::string index_name) {
    bool exists = false;
    for (unsigned int i = 0; i < tables.size(); ++i) {
        for (unsigned int j = 0; j < tables[i].ids().size(); ++j) {
            if (tables[i].ids()[j].name() == index_name) {
                exists = true;
            }
        }
    }
    return exists;
}*/

attribute* table::get_attribute(std::string name) {
    for (unsigned int i = 0; i < attributes.size(); ++i) {
        if (attributes[i].attr_name() == name) {
            return &attributes[i];
        }
    }
    return NULL;
}

int table::get_attribute_index(std::string name) {
    for (unsigned int i = 0; i < attributes.size(); ++i) {
        if (attributes[i].attr_name() == name) {
            return i;
        }
    }
    return -1;
}
