#ifndef RUNTIME_ERROR_HPP
#define RUNTIME_ERROR_HPP

#include "stdexcept"
#include "Token.hpp"

class RuntimeError : public std::runtime_error {
    public:
        Token token;

        RuntimeError(Token token, std::string message) : 
        token(token), runtime_error(message) {}
};

#endif