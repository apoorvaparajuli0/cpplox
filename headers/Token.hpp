#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "string"
#include "variant"
#include "sstream"
#include "any"
#include "typeinfo"

#include "TokenType.hpp"
#include "LoxCallable.hpp"

using Object = std::variant<std::nullptr_t, std::string, double, bool, call_ptr>;

class Token {
    public:
        TokenType type;
        std::string lexeme;
        Object literal;
        int line;

    public:
        Token();
        Token(TokenType type, std::string& lexeme, Object& literal, int line);
        std::string toString();

    template<typename T>
    static std::string VariantResolver(T var_literal) {
        std::stringstream to_str;
        to_str << var_literal;
        if(to_str.str() == "0" && (typeid(var_literal) == typeid(bool))) {
            to_str.clear(), to_str.str(std::string()), to_str << "false";
        } else if(to_str.str() == "1" && (typeid(var_literal) == typeid(bool))) {
            to_str.clear(), to_str.str(std::string()), to_str << "true";
        }
        return to_str.str();
    }

    struct Resolver {
        std::string operator()(std::nullptr_t empty_literal) {
            (void)empty_literal;
            return "null";
        };

        std::string operator()(bool var_literal) const { return VariantResolver(var_literal); }
        std::string operator()(double var_literal) const { return VariantResolver(var_literal); }
        std::string operator()(std::string var_literal) const { return VariantResolver(var_literal); }
        std::string operator()(call_ptr var_literal) const { return VariantResolver(var_literal); }
    };

    template<typename T>
    static std::any ResolveValue(T& var_literal) {
        return var_literal;
    }

    struct ValueResolver {
        std::any operator()(std::nullptr_t empty_literal) {
            (void)empty_literal;
            return empty_literal;
        }

        std::any operator()(bool var_literal) const { return ResolveValue(var_literal); }
        std::any operator()(double var_literal) const { return ResolveValue(var_literal); }
        std::any operator()(std::string var_literal) const { return ResolveValue(var_literal); }
        std::any operator()(call_ptr var_literal) const { return ResolveValue(var_literal); }
    };

    template<typename T>
    static const std::type_info& ResolveType(T var_literal) {
        return typeid(var_literal);
    }

    struct TypeResolver {
        const std::type_info& operator()(std::nullptr_t empty_literal) {
            return typeid(empty_literal); 
        }

        const std::type_info& operator()(bool var_literal) const { return ResolveType(var_literal); }
        const std::type_info& operator()(double var_literal) const { return ResolveType(var_literal); }
        const std::type_info& operator()(std::string var_literal) const { return ResolveType(var_literal); }
        const std::type_info& operator()(call_ptr var_literal) const { return ResolveType(var_literal); }
    };
};

#endif