#ifndef LOXCLASS_HPP
#define LOXCLASS_HPP

#include "memory"
#include "unordered_map"
#include "string"
#include "vector"

#include "LoxCallable.hpp"
#include "Token.hpp"

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
        std::unordered_map<std::string, call_ptr> methods;
        call_ptr superclass;

        LoxClass(std::string name, call_ptr superclass, std::unordered_map<std::string, call_ptr>& methods) : 
        name(name), superclass(superclass), methods(methods) 
        {}

        call_ptr findMethod(std::string name);
        std::string toString() override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
        int arity() override;
};

using class_ptr = std::shared_ptr<LoxClass>;

#endif