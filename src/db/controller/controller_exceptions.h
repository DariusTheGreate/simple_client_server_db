#ifndef CONTROLLER_EXCEPTIONS_H_
#define CONTROLLER_EXCEPTIONS_H_

#include <exception>

class SyntaxErrorException : public std::exception {};

class NoDatabaseSelectedException : public std::exception {};

class DatabaseNotExistException : public std::exception {};

class DatabaseAlreadyExistsException : public std::exception {};

class TableNotExistException : public std::exception {};

class TableAlreadyExistsException : public std::exception {};

class IndexAlreadyExistsException : public std::exception {};

class IndexNotExistException : public std::exception {};

class OneIndexEachTableException : public std::exception {};

class BPlusTreeException : public std::exception {};

class IndexMustBeCreatedOnPKException : public std::exception {};

class PrimaryKeyConflictException : public std::exception {};

class NoCompilationFileException : public std::exception {};

class Truble : public std::exception {};

class MemoryAllocationException : public std::exception {};


#endif