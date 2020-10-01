#include "client.h"

Client::Client()
{
	this->ConnectSocket = INVALID_SOCKET;
	this->result = NULL;
	this->ptr = NULL;
	this->recvbuflen = DEFAULT_BUFLEN;
	initializeWinsock();
	resolveServerNPort("127.0.0.1", DEFAULT_PORT);
	connectToServerAddress();
}

Client::~Client()
{
	this -> shutdownSock(this->ConnectSocket);
	//this -> recieveData();
	this -> socketCleanup(this->ConnectSocket);
}

void Client::connectToServerAddress()
{
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		this->ConnectSocket = socket(this->ptr->ai_family, this->ptr->ai_socktype, this->ptr->ai_protocol);
		if (this->ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld.\n", WSAGetLastError());
			this->socketCleanup(this -> ConnectSocket);
			throw ConnectionToServerException();
		}
		// Connect to server.
		this->iResult = connect(this->ConnectSocket, this->ptr->ai_addr, (int)this->ptr->ai_addrlen);
		if (this->iResult == SOCKET_ERROR)
		{
			closesocket(this->ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(this->result);
	if (this->ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		this->socketCleanup(this->ConnectSocket);
		throw ConnectionToServerException();
	}
}

void Client::recieveData()
{
	bool is_recived = false;

	do
	{
		this->iResult = recv(this->ConnectSocket, this->recvbuf, this->recvbuflen, 0);
		
		if (this->iResult > 0)
		{
			std::string msg(iResult, ' ');

			for (int i = 0; i < iResult; i++)
			{
				msg[i] = recvbuf[i];
			}
			std::cout << msg << std::endl;
		}
		else if (this->iResult == 0)
			printf("Connection closed.\n");
		else
		{
			printf("recv failed with error: %d.\n", WSAGetLastError());
			throw ReceiveException();
		}
	} while (is_recived);
}

void Client::sending(std::string msg)
{
	try{
		sendData(std::move(msg), ConnectSocket);
	}
	catch(SendDataException e){
		this->socketCleanup(this->ConnectSocket);
	}

	recieveData();
}
