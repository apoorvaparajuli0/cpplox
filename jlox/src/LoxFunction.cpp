#include "../headers/LoxFunction.hpp"
#include "../headers/Stmt.hpp"
#include "../headers/LoxClass.hpp"
#include "../headers/ReturnError.hpp"
#include "../headers/Interpreter.hpp"

LoxFunction::LoxFunction(const Function* declaration, env_ptr& closure, bool isInitializer) {
    this->declaration = std::shared_ptr<const Stmt>(declaration);
    this->closure = closure;
    this->isInitializer = isInitializer;
}

LoxFunction::LoxFunction(const stmt_ptr& declaration, env_ptr& closure, bool isInitializer) {
    this->declaration = declaration;
    this->closure = closure;
    this->isInitializer = isInitializer;
}

call_ptr LoxFunction::bind(LoxInstance instance) {
    env_ptr environment(new Environment(closure));
    instance_ptr inst_ptr(new LoxInstance(instance));

    environment->define("this", inst_ptr);

    return call_ptr(new LoxFunction(dynamic_cast<const Function*>(declaration.get()), environment, isInitializer));
}

std::string LoxFunction::toString() {
    return "<fn" + dynamic_cast<const Function*>(declaration.get())->name.lexeme + ">";
}

int LoxFunction::arity() {
    return dynamic_cast<const Function*>(declaration.get())->params.size();
}

Object LoxFunction::call(Interpreter& interpreter, std::vector<Object>& arguments) {
    env_ptr environment = env_ptr(new Environment(closure));

    for (int i = 0; i < dynamic_cast<const Function*>(declaration.get())->params.size(); i++) {
        environment->define(dynamic_cast<const Function*>(declaration.get())->params.at(i).lexeme, arguments.at(i));
    }

    try {
        interpreter.executeBlock(dynamic_cast<const Function*>(declaration.get())->body, environment);
    } catch(const ReturnError& err) {
        if(isInitializer) return closure->getAt(0, "this");
        return err.value;
    }

    if(isInitializer) return closure->getAt(0, "this");

    return std::nullptr_t{};
}
