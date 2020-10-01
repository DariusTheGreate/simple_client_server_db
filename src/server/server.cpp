#include "server.h"

Server::Server(int port, int buf_len) : default_buflen(buf_len), default_port(port)
{
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    iSendResult;
    recvbuf = new char[DEFAULT_BUFLEN];
    recvbuflen = DEFAULT_BUFLEN;


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw Truble();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

Server::~Server()
{

}

void Server::start()
{
    int iResult;
    if (getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0)
    {
        WSACleanup();
        throw Truble();
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld \n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        throw Truble();
    }

    // Setup the TCP listening socket
    if (::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        throw Truble();
    }
    freeaddrinfo(result);

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %d \n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        throw Truble();
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("accept failed with error: %d \n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        throw Truble();
    }

    // No longer need server socket
    closesocket(ListenSocket);
    // Receive until the peer shuts down the connection

    // Receive until the peer shuts down the connection
    //std::fstream file;

    while(command != std::string("quit"))
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
            //std::cout << command << std::endl;
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
            system("PAUSE");
            throw Truble();
        }
    }

    stop();
}

void Server::stop()
{
    int iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        throw Truble();
    }
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
    //make sure you resolve the filename to the correct name listed on the first line of the file.
    std::ifstream inFile;
    std::fstream dummyFile;
    std::string realFileName;
    //open the file
    inFile.open("newEntry.txt", std::ios::in | std::ios::binary);
    if (!inFile.is_open() && !inFile.good())//if inFile isn't good, leave quickly.
    {
        std::cout << "File unfound, not good or not open... or all of them. Terminating program." << std::endl;
        throw Truble();
    }
    //get the size of the file
    inFile.seekg(0, inFile.end);
    int fileSize = inFile.tellg();
    inFile.seekg(0, inFile.beg);
    char* fileBuffer = new char[fileSize];
    //load file into buffer
    inFile.read(fileBuffer, fileSize);
    inFile.seekg(0, inFile.beg);
    //get filename from first line.
    getline(inFile, realFileName);
    inFile.close();
    std::cout << realFileName << " is the name of the file." << std::endl;
    dummyFile.open(realFileName, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!dummyFile.is_open())
    {
        printf("File unable to be opened.\n");
        throw Truble();
    }
    for (int i = realFileName.length(); i < fileSize; i++)
    {
        dummyFile << fileBuffer[i];
    }
    //cleanup
    delete[] fileBuffer;
    dummyFile.close();
    delete recvbuf;
}


void Server::execute()
{
    Interpreter itp;
    std::cout << "com is " << command << std::endl;
    itp.ExecSQL(command);
}