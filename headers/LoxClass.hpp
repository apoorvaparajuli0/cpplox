#ifndef LOXCLASS_HPP
#define LOXCLASS_HPP

#include "unordered_map"
#include "string"

#include "LoxCallable.hpp"

class LoxInstance {
    private:
        std::shared_ptr<LoxClass> klass;
        std::unordered_map<std::string, Object> fields;

    public:
        LoxInstance(LoxClass& klass);
        std::string toString();
        Object get(Token name);
        void set(Token name, Object value);
};

class LoxClass : public LoxCallable {
    public:
        std::string name;

        LoxClass(std::string name) : name(name) {}

        std::string toString() override {
            return name;
        }

        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override {
            std::shared_ptr<LoxInstance> instance(new LoxInstance(*this));
            return instance;
        }

        int arity() override {
            return 0;
        }

};

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

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, Object value) {
    fields[name.lexeme] = value;
}

using class_ptr = std::shared_ptr<LoxClass>;

#endif