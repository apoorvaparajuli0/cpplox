#ifndef RETURNERROR_HPP
#define RETURNERROR_HPP

#include "stdexcept"

class ReturnError : public std::runtime_error {
    public:
        Object value;

        ReturnError(Object value) : 
        runtime_error("return"), value(value) {}
};

#endif