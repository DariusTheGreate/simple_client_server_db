#ifndef CLIENT_EXCEPTIONS_H_
#define CLIENT_EXCEPTIONS_H_

#include <exception>

class ConnectionToServerException : public std::exception {};

class ReceiveException : public std::exception {};

#endif