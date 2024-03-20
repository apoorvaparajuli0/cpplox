#include "string"
#include "variant"
#include "sstream"
#include "TokenType.hpp"

using Object = std::variant<std::nullptr_t, std::string, double, bool>;

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
        return to_str.str();
    }

    template<typename T>
    static std::string ResolveType(T var_literal) {
        std::stringstream to_str;
        to_str << var_literal;
        if(to_str.str() == "true" || to_str.str() == "false") return "bool";

        std::string out_str = to_str.str();
        for(char c : out_str) {
            if(!(c >= 48 && c <= 57)) {
                return "string";
            } else if((c == out_str.back()) && (c >= 48 && c <= 57)) {
                return "number";
            }
        }

        return to_str.str();
    }

    struct TypeResolver {
        std::string operator()(std::nullptr_t empty_literal) {
            (void)empty_literal;
            return "null";
        };

        std::string operator()(bool var_literal) const { return ResolveType(var_literal); }
        std::string operator()(double var_literal) const { return ResolveType(var_literal); }
        std::string operator()(std::string var_literal) const { return ResolveType(var_literal); }
    };

    struct Resolver {
        std::string operator()(std::nullptr_t empty_literal) {
            (void)empty_literal;
            return "null";
        };

        std::string operator()(bool var_literal) const { return VariantResolver(var_literal); }
        std::string operator()(double var_literal) const { return VariantResolver(var_literal); }
        std::string operator()(std::string var_literal) const { return VariantResolver(var_literal); }
    };
};