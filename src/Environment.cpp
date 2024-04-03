#include "../headers/Environment.hpp"
#include "../headers/RuntimeError.hpp"

#include "utility"
#include "ranges"

void Environment::define(std::string name, const Object& value) {
    values[name] = value;
}

env_ptr Environment::ancestor(int distance) {
    env_ptr environment(this);
    for(int i = 0; i < distance; i++) {
        environment = environment.get()->enclosing;
    }

    return environment;
}

Object Environment::getAt(int distance, std::string name) {
    return ancestor(distance).get()->values.at(name);
}

void Environment::assignAt(int distance, Token name, const Object& value) {
    ancestor(distance).get()->values[name.lexeme] = value;
}

Object Environment::get(Token name) {
    //CHALLENGE 8.2: Make it an Error to Access an Uninitialized Variable
    if(values.contains(name.lexeme)/* && !(std::visit(Token::Resolver{}, values[name.lexeme]) == "null")) */) {
        return values.at(name.lexeme);
    }

    if(enclosing != std::nullptr_t{}) return enclosing.get()->get(name);

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, const Object& value) {
    if(values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if(enclosing != std::nullptr_t{}) enclosing.get()->assign(name, value); return;

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}