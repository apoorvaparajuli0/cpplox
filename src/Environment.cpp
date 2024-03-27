#include "../headers/Environment.hpp"

void Environment::define(std::string name, Object value) {
    values.insert({name, value});
}

Object Environment::get(Token name) {
    if(values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }

    if(enclosing != std::nullptr_t{}) return enclosing.get()->get(name);

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, Object value) {
    if(values.contains(name.lexeme)) {
        values.insert({name.lexeme, value});
        return;
    }

    if(enclosing != std::nullptr_t{}) enclosing.get()->assign(name, value); return;

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}