#ifndef LOXCALLABLE_HPP
#define LOXCALLABLE_HPP

#include "any"
#include "iostream"
#include "vector"

class Interpreter;

class LoxCallable {
    public:
        virtual int arity() = 0;
        virtual std::variant<std::nullptr_t, std::string, double, bool, std::shared_ptr<LoxCallable>> 
        call(Interpreter& interpreter, std::vector<std::variant<std::nullptr_t, std::string, double, bool, std::shared_ptr<LoxCallable>>> arguments) = 0;
        virtual std::string toString() = 0;
        virtual ~LoxCallable() = default;

    friend std::ostream& operator<<(std::ostream& os, const LoxCallable& rhs) {
        os << "LoxCallable";
        return os;
    }
};

using call_ptr = std::shared_ptr<LoxCallable>;

#endif