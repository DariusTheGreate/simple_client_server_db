#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1"

#include <compiler\interpreter.cc>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <iostream>
#include "exceptions.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1"
#pragma comment (lib, "Ws2_32.lib")

class Server
{
private:
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	struct addrinfo* result;
	struct addrinfo hints;
	int iSendResult;
	char* recvbuf;
	int recvbuflen;
	int default_port;
	int default_buflen;
	std::string command;
	void stop();
public:
	void execute();
	Server(int port = 1, int buf_len = 512);
	~Server();
	void start();
	std::string get_command();
};

