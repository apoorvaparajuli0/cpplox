#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "unordered_map"
#include "string"
#include "../headers/Token.hpp"
#include "../headers/RuntimeError.hpp"

class Environment {
    public:
        void define(std::string name, Object value);
        Object get(Token name);

    private:
        std::unordered_map<std::string, Object> values;
};

#endif