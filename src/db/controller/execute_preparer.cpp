
#include "execute_preparer.h"
#include "../includes/commons.h"


int execute_preparer::parse_data_type(std::vector<std::string> sql_vector, attribute& attr, unsigned int pos) {
    boost::algorithm::to_lower(sql_vector[pos]);

    if (sql_vector[pos] == "int") {
        std::cout << "TYPE: "
            << "int" << std::endl;
        attr.set_data_type(T_INT);
        attr.set_length(4);
        pos++;
        if (sql_vector[pos] == ",") {
            pos++;
        }
    }
    else if (sql_vector[pos] == "float") {
        std::cout << "TYPE: "
            << "float" << std::endl;
        attr.set_data_type(T_FLOAT);
        attr.set_length(4);
        pos++;
        if (sql_vector[pos] == ",") {
            pos++;
        }
    }
    else if (sql_vector[pos] == "char") {
        attr.set_data_type(T_CHAR);
        pos++;
        if (sql_vector[pos] == "(") {
            pos++;
        }
        attr.set_length(atoi(sql_vector[pos].c_str()));
        pos++;
        if (sql_vector[pos] == ")") {
            pos++;
        }
        if (sql_vector[pos] == ",") {
            pos++;
        }
    }
    else {
        throw SyntaxErrorException();
    }

    return pos;
}

execute_database_creator::execute_database_creator(const std::vector<std::string>&& command)
{
    if (command.size() <= 2) {
        throw SyntaxErrorException();
    }
    else {
        db_name_ = command[2];
    }
}

execute_database_droper::execute_database_droper(const std::vector<std::string>&& command)
{
    if (command.size() <= 2) {
        throw SyntaxErrorException();
    }
    else {
        db_name = command[2];
    }
}

execute_database_user::execute_database_user(const std::vector<std::string>&& command)
{
    if (command.size() <= 1) {
        throw SyntaxErrorException();
    }
    else {
        db_name = command[1];
    }
}

execute_database_tablecreator::execute_database_tablecreator(const std::vector<std::string>&& command)
{
    output_stream* out = output_stream::GetInstance("");
    unsigned int pos = 2;
    bool is_attr = true;

    if (command.size() <= pos) {
        throw SyntaxErrorException();
    }
    out->set_value_to("TABLE NAME: " + command[pos] + "\n");
    std::cout << "TABLE NAME: " << command[pos] << std::endl;
    tb_name = command[pos];
    pos++;

    if (command[pos] != "(") {
        throw SyntaxErrorException();
    }
    pos++;

    bool has_pk = false;

    while (is_attr) {
        is_attr = false;

        if (command[pos] == "primary") {
            pos++;
            if (command[pos] != "key") {
                throw SyntaxErrorException();
            }
            pos++;

            if (has_pk) {
                throw SyntaxErrorException();
            }

            if (command[pos] != "(") {
                throw SyntaxErrorException();
            }
            pos++;
            for (unsigned int i = 0; i < _attrs_.size(); ++i) {
                if (_attrs_[i].attr_name() == command[pos]) {
                    _attrs_[i].set_attr_type(1);
                    std::cout << "PRIMARY KEY: " << command[pos] << std::endl;
                    out->append_to_value("PRIMARY KEY: " + command[pos]+"\n");
                }
            }
            pos++;
            if (command[pos] != ")") {
                throw SyntaxErrorException();
            }
            has_pk = true;
        }
        else {
            std::cout << "COLUMN: " << command[pos] << std::endl;
            out->append_to_value("COLUMN: " + command[pos] + "\n");
            attribute attr;
            attr.set_attr_name(command[pos]);
            pos++;

            pos = parse_data_type(command, attr, pos);

            _attrs_.push_back(attr);

            if (command[pos] != ")") {
                is_attr = true;
            }
        }
    }
}
execute_table_droper::execute_table_droper(const std::vector<std::string>&& command)
{
    if (command.size() <= 2) {
        throw SyntaxErrorException();
    }
    else {
        tb_name = command[2];
    }
}


execute_insert_preparer::execute_insert_preparer(const std::vector<std::string>&& sql_vector) {
    unsigned int pos = 1;
    bool is_attr = true;

    if (boost::to_lower_copy(sql_vector[pos]) != "into") {
        throw SyntaxErrorException();
    
    }
    
    pos++;
    std::cout << "TABLE NAME: " << sql_vector[pos] << std::endl;
    tb_name_ = sql_vector[pos];
    pos++;
    if (boost::to_lower_copy(sql_vector[pos]) != "values") {
        throw SyntaxErrorException();
    }
    pos++;
    if (boost::to_lower_copy(sql_vector[pos]) != "(") {
        throw SyntaxErrorException();
    }
    pos++;
    while (is_attr) {
        is_attr = false;
        value sql_value;
        std::string value = sql_vector[pos];
        if (value.at(0) == '\'' || value.at(0) == '\"') {
            value.assign(value, 1, value.length() - 2);
            sql_value.data_type = 2;
        }
        else {
            if (value.find(".") != std::string::npos) {
                sql_value.data_type = 1;
            }
            else {
                sql_value.data_type = 0;
            }
        }
        sql_value.value_ = value;
        std::cout << sql_value.data_type << " : " << value << std::endl;
        pos++;
        values_.push_back(sql_value);
        if (sql_vector[pos] != ")") {
            is_attr = true;
        }
        pos++;
    }
}