#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "memory"
#include "string"
#include "unordered_map"

#include "LoxCallable.hpp"
#include "Token.hpp"

class Environment {
    public:
        std::shared_ptr<Environment> enclosing;

        Environment() : enclosing(std::nullptr_t{}) {}
        Environment(std::shared_ptr<Environment>& enclosing) {
            this->enclosing = enclosing;
        }

        void define(std::string name, const Object& value);
        std::shared_ptr<Environment> ancestor(int distance);
        Object getAt(int distance, std::string name);
        void assignAt(int distance, Token name, const Object& value);
        Object get(Token name);
        void assign(Token name, const Object& value);

    private:
        std::unordered_map<std::string, Object> values;
};

using env_ptr = std::shared_ptr<Environment>;

#endif