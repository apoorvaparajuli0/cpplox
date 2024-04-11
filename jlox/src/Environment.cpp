#include "../headers/Environment.hpp"
#include "../headers/RuntimeError.hpp"

void Environment::define(std::string name, const Object& value) {
    values[name] = value;
}

env_ptr Environment::ancestor(int distance) {
    env_ptr environment(new Environment(*this));
    for(int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }

    return environment;
}

Object Environment::getAt(int distance, std::string name) {
    return ancestor(distance)->values.at(name);
}

void Environment::assignAt(int distance, Token name, const Object& value) {
    ancestor(distance)->values[name.lexeme] = value;
}

Object Environment::get(Token name) {
    if(values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }

    if(enclosing != std::nullptr_t{}) return enclosing->get(name);

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, const Object& value) {
    if(values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if(enclosing != std::nullptr_t{}) enclosing->assign(name, value); return;

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}