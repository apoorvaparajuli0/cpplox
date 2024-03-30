#include "../headers/Environment.hpp"
#include "../headers/RuntimeError.hpp"

void Environment::define(std::string name, Object value) {
    values[name] = value;
}

Object Environment::get(Token name) {
    //CHALLENGE 8.2: Make it an Error to Access an Uninitialized Variable
    if(values.contains(name.lexeme)/* && !(std::visit(Token::Resolver{}, values[name.lexeme]) == "null")) */) {
        return values.at(name.lexeme);
    }

    if(enclosing != std::nullptr_t{}) return enclosing.get()->get(name);

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, Object value) {
    if(values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    if(enclosing != std::nullptr_t{}) enclosing.get()->assign(name, value); return;

    throw RuntimeError(name, "Undefined Variable '" + name.lexeme + "'.");
}