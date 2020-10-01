#include "parser.h"

using namespace std;

parser::parser(std::string command) : sql_type_(-1) {
    char* libvar;
    size_t requiredSize = 100;

    libvar = (char*)malloc(requiredSize * sizeof(char));
    if (!libvar) throw MemoryAllocationException();
    
    getenv_s(&requiredSize, libvar, requiredSize, "HOME");//cahnge
    path_location = string(libvar) + "/MiniDBData";
    sql_statement_ = command;
    
    command_types.resize(21);
    command_types.push_back(command_info(10, "quit"));
    command_types.push_back(command_info(20, "help"));
    command_types.push_back(command_info(30, "exec"));
    command_types.push_back(command_info(40, "create database"));
    command_types.push_back(command_info(50, "show database"));
    command_types.push_back(command_info(60, "use"));
    command_types.push_back(command_info(70, "drop database"));
    command_types.push_back(command_info(80, "create table"));
    command_types.push_back(command_info(90, "show tables"));
    command_types.push_back(command_info(100, "drop tables"));
    command_types.push_back(command_info(110, "create index"));
    command_types.push_back(command_info(120, "select"));
    command_types.push_back(command_info(130, "insert"));
    command_types.push_back(command_info(140, "delete"));
    command_types.push_back(command_info(150, "update"));
    command_types.push_back(command_info(0, ""));
}

parser::~parser() { }

void parser::parse()
{
    FormatSQL();
    TellSQLType();
}

vector<string> split(string str, string sep) {
    char* cstr = const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current = strtok(cstr, sep.c_str());
    while (current != NULL) {
        arr.push_back(current);
        current = strtok(NULL, sep.c_str());
    }
    return arr;
}


void parser::FormatSQL() {
    boost::regex reg("[\r\n\t]");
    sql_statement_ = boost::regex_replace(sql_statement_, reg, " ");
    reg = ";.*$";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, "");
    reg = "(^ +)|( +$)";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, "");
    reg = " +";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, " ");
    reg = " ?(\\(|\\)|,|=|(<>)|<|>) ?";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, " $1 ");
    reg = "< *>";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, "<>");
    reg = "< *=";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, "<=");
    reg = "> *=";
    sql_statement_ = boost::regex_replace(sql_statement_, reg, ">=");
    sql_vector_ = split(sql_statement_, " ");
}

void parser::TellSQLType() {
    if (sql_vector_.size() == 0) {
        sql_type_ = -1;
        return;
    }
    boost::algorithm::to_lower(sql_vector_[0]);
    if (sql_vector_[0] == "quit") {
        sql_type_ = 10;
    }
    else if (sql_vector_[0] == "help") {
        sql_type_ = 20;
    }
    else if (sql_vector_[0] == "create") {
        boost::algorithm::to_lower(sql_vector_[1]);
        if (sql_vector_[1] == "database") {
            sql_type_ = 30;
        }
        else if (sql_vector_[1] == "table") {
            cout << "sql type: 'create table' " << endl;
            sql_type_ = 31;
        }
        else if (sql_vector_[1] == "index") {
            cout << "sql type: 'create index' " << endl;
            sql_type_ = 32;
        }
        else {
            sql_type_ = -1;
        }
    }
    else if (sql_vector_[0] == "show") {
        boost::algorithm::to_lower(sql_vector_[1]);
        if (sql_vector_[1] == "databases") {
            cout << "sql type: 'show databases' " << endl;
            sql_type_ = 40;
        }
        else if (sql_vector_[1] == "tables") {
            cout << "sql type: 'show tables' " << endl;
            sql_type_ = 41;
        }
        else {
            sql_type_ = -1;
        }
    }
    else if (sql_vector_[0] == "drop") {
        boost::algorithm::to_lower(sql_vector_[1]);
        if (sql_vector_[1] == "database") {
            cout << "sql type: 'drop database' " << endl;
            sql_type_ = 50;
        }
        else if (sql_vector_[1] == "table") {
            cout << "sql type: 'drop table' " << endl;
            sql_type_ = 51;
        }
        else if (sql_vector_[1] == "index") {
            cout << "sql type: 'drop index' " << endl;
            sql_type_ = 52;
        }
        else {
            sql_type_ = -1;
        }
    }
    else if (sql_vector_[0] == "use") {
        cout << "sql type: 'use' " << endl;
        sql_type_ = 60;
    }
    else if (sql_vector_[0] == "insert") {
        cout << "sql type: 'insert' " << endl;
        sql_type_ = 70;
    }
    else if (sql_vector_[0] == "exec") {
        cout << "sql type: 'exec' " << endl;
        sql_type_ = 80;
    }
    else if (sql_vector_[0] == "select") {
        cout << "sql type: 'select' " << endl;
        sql_type_ = 90;
    }
    else if (sql_vector_[0] == "delete") {
        cout << "sql type: 'delete' " << endl;
        sql_type_ = 100;
    }
    else if (sql_vector_[0] == "update") {
        cout << "sql type: 'update' " << endl;
        sql_type_ = 110;
    }
    else {
        sql_type_ = -1;
        cout << "sql type: 'unknown' " << endl;
    }
    if (sql_type_ == -1) {
        std::string str;
        for (unsigned int i = 0; i < sql_vector_.size(); i++)
            str += sql_vector_[i];
        command_info sql_type_prediction = get_prediction(str);
        cout << "sql type: 'unknown'" << " maybe u mean " << sql_type_prediction.command << endl;
    }
}

int get_levenstein_distance(std::string input, std::string candidat)
{
    const char* s1 = input.c_str();
    const char* s2 = candidat.c_str();
    int l1 = strlen(input.c_str());
    int l2 = strlen(s2);
    if (l1 > 50 || l2 > 50)
        return 0;
    int i, j, t, track;
    int dist[50][50];
    for (i = 0; i <= l1; i++)
        dist[0][i] = i;
    for (j = 0; j <= l2; j++)
        dist[j][0] = j;

    for (j = 1; j <= l1; j++)
        for (i = 1; i <= l2; i++) {
            if (s1[i - 1] == s2[j - 1]) {
                track = 0;
            }
            else {
                track = 1;
            }
            t = MIN((dist[i - 1][j] + 1), (dist[i][j - 1] + 1));
            dist[i][j] = MIN(t, (dist[i - 1][j - 1] + track));
        }
    return dist[l2][l1];
}

//~O(n^3)+ -> change command_types to map
command_info parser::get_prediction(std::string input)
{
    command_info best_coincidence;
    int best_score = 1000;
    for (auto i = command_types.begin(); i < command_types.end(); i++)
    {
        int dist = get_levenstein_distance(input, i->command);

        if (dist < best_score)
        {
            best_score = dist;
            best_coincidence.command = i->command;
            best_coincidence.sql_type = i->sql_type;
        }
    }

    return best_coincidence;
}