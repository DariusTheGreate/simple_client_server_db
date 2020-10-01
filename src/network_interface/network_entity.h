#define WIN32_LEAN_AND_MEAN
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
#include "network_exceptions.h"

class NetworkEntity
{
public:
	WSADATA wsaData;
	SOCKET ConnectSocket;

	SOCKET ClientSocket;
	struct addrinfo* result;
	struct addrinfo hints;
	struct addrinfo* ptr;
	
	int recvbuflen;
	int default_port;
	int default_buflen;
	int iResult;

	void initializeWinsock();
	void resolveServerNPort(PCSTR ip, PCSTR port);
	
	void shutdownSock(SOCKET sock);
	void socketCleanup(SOCKET sock);

	
	void sendData(std::string&& fileBuffer, SOCKET sock);
};