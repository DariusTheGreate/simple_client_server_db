#include "interpreter.h"
#define MIN(x, y) ((x) < (y) ? (x) : (y)) 
#include <fstream>
#include <iostream>
#include "..\singletones\output_stream.h"

using namespace std;

Interpreter::~Interpreter() { }
Interpreter::Interpreter(parser* p) : parser_(p)
{
    executer = new execute_controller(p->get_path(), p->get_command());
}
void Interpreter::Run() 
{
    try
    {
        switch (parser_->get_state()) {
            case 10: {
                executer->Quit();
                exit(0);
            } break;
            case 20: {
                executer->Help();
            } break;
            case 30: {
                executer->CreateDatabase();
            }break;
            case 40: {
                executer->ShowDatabases();
            }break;
            case 50: {
                executer->DropDatabase();
            }break;
            case 60: {
                executer->Use();
            }break;
            case 41: {
                executer->ShowTables();
            }break;
            case 31: {
                executer->CreateTable();
            }break;
            case 70: {
                executer->Insert();
            }break;
            case 51: {
                executer->DropTable();
            }break;
            case -1: {
                executer->EmptyCommand();
            }
        }
    }
    catch (SyntaxErrorException e)
    {
        output_stream* single_wolf = output_stream::GetInstance("\n");
        single_wolf->append_to_value(" !!!syntax is bad!!!");
    }
    catch (DatabaseNotExistException e)
    {
        output_stream* single_wolf = output_stream::GetInstance("\n");
        single_wolf->append_to_value(" !!!database does not exsist!!!");
    }
    catch (DatabaseAlreadyExistsException e)
    {
        output_stream* single_wolf = output_stream::GetInstance("\n");
        single_wolf->append_to_value(" !!!database already exsist!!!");
    }
    catch (NoDatabaseSelectedException e)
    {
        output_stream* single_wolf = output_stream::GetInstance("\n");
        single_wolf->append_to_value(" !!!database not selected!!!");
    }
    catch (TableNotExistException e)
    {
        output_stream* single_wolf = output_stream::GetInstance("\n");
        single_wolf->append_to_value(" !!!table not exsist!!!");
    }
    catch (TableAlreadyExistsException e)
    {
        output_stream* single_wolf = output_stream::GetInstance("\n");
        single_wolf->append_to_value(" !!!table already exsist!!!");
    }
}

std::string Interpreter::get_response()
{
    std::cout << "RESPONSEBLE\n";
    output_stream* single_wolf = output_stream::GetInstance("");
    return single_wolf->value();
}