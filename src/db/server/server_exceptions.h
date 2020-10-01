#ifndef SERVER_EXCEPTIONS_H_
#define SERVER_EXCEPTIONS_H_

#include <exception>


class CreateSocketException : public std::exception {};

class ListeningSocketException : public std::exception {};

class AcceptSocketException : public std::exception {};

class RecvSocketException : public std::exception {};

#endif