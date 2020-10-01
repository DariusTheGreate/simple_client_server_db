#include <network_exceptions.h>
#include "client_exceptions.h"
#include <boost/algorithm/string.hpp>
#include <readline/history.h>
#include <readline/readline.h>

#include <network_entity.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib" )
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1"

class Client : public NetworkEntity
{
private:
	char recvbuf[DEFAULT_BUFLEN];
	void connectToServerAddress();
	void recieveData();
	void stop();
public:
	Client();
	~Client();
	void sending(std::string msg);
};

