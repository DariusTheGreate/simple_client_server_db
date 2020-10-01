#include "client.h"

int main()
{
	try
	{
		Client* clnt = new Client();//init;

		std::string sql;

		using_history();

		while (true) {
			sql = std::string(readline("MiniDB> "));

			boost::algorithm::trim(sql);

			if (sql.compare(0, 4, "exit") == 0 || sql.compare(0, 4, "quit") == 0) {

				clnt->sending(sql);

				break;
			}

			if (sql.length() != 0) {
				add_history(sql.c_str());
				clnt->sending(sql);
			}

			std::cout << std::endl;
		}
	}
	catch(ConnectionToServerException& e)
	{
		std::cout << "problems with connection to server\n" << std::endl;
	}
	catch (SendDataException& e)
	{
		std::cout << "cant send data to server\n" << std::endl;
	}
	catch (ShutDownSocketException& e)
	{
		std::cout << "shutdown socket exception\n" << std::endl;
	}
	catch (ResolveServerException& e)
	{
		std::cout << "resolve server exception\n" << std::endl;
	}
	catch (WSASStartException& e)
	{
		std::cout << "WSAS start exception\n" << std::endl;
	}
	catch (ReceiveException& e)
	{
		std::cout << "receive exception\n" << std::endl;
	}
	system("PAUSE");
	return 0;
}