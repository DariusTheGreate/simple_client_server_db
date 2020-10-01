#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1"

#include "..\compiler\interpreter.h"
#include <network_exceptions.h>
#include "server_exceptions.h"
#include "..\singletones\output_stream.h"
#include <network_entity.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1"
#pragma comment (lib, "Ws2_32.lib")

class Server : NetworkEntity
{
private:
	SOCKET ListenSocket;
	int iSendResult;
	char* recvbuf;
	std::string command;
	std::string response;
	bool run;
public:
	Server(int port = 1, int buf_len = 512);
	~Server();
	void execute();
	void start();
};

