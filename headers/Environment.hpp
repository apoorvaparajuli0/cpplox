#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "unordered_map"
#include "string"

#include "Token.hpp"

class Environment {
    public:
        std::shared_ptr<Environment> enclosing;

        Environment() : enclosing(std::nullptr_t{}) {}
        Environment(std::shared_ptr<Environment>& enclosing) {
            this->enclosing = enclosing;
        }

        void define(std::string name, Object value);
        Object get(Token name);
        void assign(Token name, Object value);

    private:
        std::unordered_map<std::string, Object> values;
};

using env_ptr = std::shared_ptr<Environment>;

#endif