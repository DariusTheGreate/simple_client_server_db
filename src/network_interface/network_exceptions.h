#ifndef NETWORK_EXCEPTIONS_H_
#define NETWORK_EXCEPTIONS_H_

#include <exception>


class WSASStartException : public std::exception {};

class ResolveServerException : public std::exception {};

class ShutDownSocketException : public std::exception {};

class SendDataException : public std::exception {};


#endif