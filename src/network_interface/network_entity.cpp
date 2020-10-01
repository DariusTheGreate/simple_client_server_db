#include "network_entity.h"

void NetworkEntity::initializeWinsock()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d.\n", iResult);
        system("PAUSE");
        throw WSASStartException();
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

void NetworkEntity::resolveServerNPort(PCSTR ip, PCSTR port)
{
    int iResult;
    if (getaddrinfo(ip, port, &hints, &result) != 0)
    {
        WSACleanup();
        throw ResolveServerException();
    }
}

void NetworkEntity::shutdownSock(SOCKET sock)
{
    int iResult = shutdown(sock, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        throw ShutDownSocketException();
    }
}

void NetworkEntity::socketCleanup(SOCKET sock)
{
    closesocket(sock);
    WSACleanup();
}

//templ
void NetworkEntity::sendData(std::string&& fileBuffer, SOCKET sock)
{
    iResult = send(sock, fileBuffer.c_str(), fileBuffer.size(), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed with error: %d.\n", WSAGetLastError());
        socketCleanup(sock);
        throw SendDataException();
    }
}