#include "server/server.h"
int main() 
{
	try
	{
		std::cout << "Database server is on.." << std::endl;
		Server* serv = new Server();
		serv -> start();
	}
	catch (...)
	{
		std::cout << "exception\n";
	}

	system("PAUSE");
	return 0;
}
