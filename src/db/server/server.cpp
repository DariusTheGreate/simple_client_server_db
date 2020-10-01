#include "server.h"

Server::Server(int port, int buf_len)
{
    run = true;
    default_port = buf_len;
    default_buflen = port;
    iSendResult = -1;
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    recvbuf = new char[DEFAULT_BUFLEN];
    recvbuflen = DEFAULT_BUFLEN;
    initializeWinsock();
    resolveServerNPort(NULL, DEFAULT_PORT);
}

Server::~Server() 
{
    this->shutdownSock(this -> ClientSocket);
    this->socketCleanup(this -> ClientSocket);
}

void Server::start()
{
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld \n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        throw CreateSocketException();
    }

    // Setup the TCP listening socket
    if (::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        throw ListeningSocketException();
    }
    freeaddrinfo(result);

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %d \n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        throw ListeningSocketException();
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("accept failed with error: %d \n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        throw AcceptSocketException();
    }

    closesocket(ListenSocket);
    
    while(run)
    {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        
        if (iResult > 0)
        {
            std::string msg(iResult, ' ');

            for (int i = 0; i < iResult; i++)
            {
                msg[i] = recvbuf[i];
            }
            command = msg;//bad
            execute();
        }
        else if (iResult == 0)
            printf("Connection closing... \n");
        else
        {
            printf("recv failed with error: %d \n", WSAGetLastError());
            closesocket(ClientSocket);
            WSAGetLastError();
            WSACleanup();
            throw RecvSocketException();
        }
    }
}

void Server::execute()
{
    parser p(command);

    p.parse();
    if (p.get_state() == 10) {
        run = false;

        sendData("im quit", ClientSocket);
        return;
    }

    Interpreter itp(&p);
    itp.Run();

    sendData(itp.get_response(), ClientSocket);

}
