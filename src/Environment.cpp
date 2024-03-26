#include "../headers/Environment.hpp"

void Environment::define(std::string name, Object value) {
    values.insert({name, value});
}

Object Environment::get(Token name) {
    if(values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}