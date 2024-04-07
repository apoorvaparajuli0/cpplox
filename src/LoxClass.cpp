#include "../headers/LoxClass.hpp"
#include "../headers/LoxFunction.hpp"

//LOXCLASS
call_ptr LoxClass::findMethod(std::string name) {
    if(methods.contains(name)) {
        return methods.at(name);
    }

    return std::nullptr_t{};
}

std::string LoxClass::toString() {
    return name;
}

Object LoxClass::call(Interpreter& interpreter, std::vector<Object>& arguments) {
    std::shared_ptr<LoxInstance> instance(new LoxInstance(*this));
    call_ptr initializer = findMethod("init");
    if(initializer != std::nullptr_t{}) {
        dynamic_cast<LoxFunction*>(initializer.get())->bind(*(instance.get()))->call(interpreter, arguments);
    }
    return instance;
}

int LoxClass::arity() {
    call_ptr initializer = findMethod("init");
    if(initializer == std::nullptr_t{}) return 0;

    return dynamic_cast<LoxFunction*>(initializer.get())->arity();
}

//LOXINSTANCE
LoxInstance::LoxInstance(LoxClass& klass) {
    this->klass = std::shared_ptr<LoxClass>(new LoxClass(klass));
}

std::string LoxInstance::toString() {
    return klass->name + " instance";
}

Object LoxInstance::get(Token name) {
    if (fields.contains(name.lexeme)) {
      return fields.at(name.lexeme);
    }

    call_ptr method = klass->findMethod(name.lexeme);
    if(method != std::nullptr_t{}) return dynamic_cast<LoxFunction*>(method.get())->bind(*this);

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, Object value) {
    fields[name.lexeme] = value;
}