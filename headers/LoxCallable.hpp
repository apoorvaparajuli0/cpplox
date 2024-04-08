#ifndef LOXCALLABLE_HPP
#define LOXCALLABLE_HPP

#include "any"
#include "iostream"
#include "vector"

class Interpreter;

class LoxCallable;
class LoxClass;
class LoxInstance;

using call_ptr = std::shared_ptr<LoxCallable>;
using class_ptr = std::shared_ptr<LoxClass>;
using instance_ptr = std::shared_ptr<LoxInstance>;

using Object = std::variant<std::nullptr_t, std::string, double, bool, call_ptr, instance_ptr>;

class LoxCallable {
    public:
        virtual int arity() = 0;
        virtual Object call(Interpreter& interpreter, std::vector<Object>& arguments) = 0;
        virtual std::string toString() = 0;
        virtual ~LoxCallable() = default;
};

#endif