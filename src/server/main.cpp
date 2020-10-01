#include "server.h"

int main() 
{
	try
	{
		Server* serv = new Server();
		serv -> start();
	}
	catch(...)//DANGER add exception controller
	{
		std::cout << "cant ident exception" << std::endl;
	}
	return 0;
}
