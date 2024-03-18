#include "string"
#include "variant"
#include "sstream"
#include "TokenType.hpp"

using Object = std::variant<std::nullptr_t, std::string, double, bool>;

class Token {
    TokenType type;
    std::string lexeme;
    Object literal;
    int line;

    public:
        Token();
        Token(TokenType type, std::string &lexeme, Object &literal, int line);
        std::string toString();

    template<typename T>
    static std::string VariantResolver(T var_literal) {
        std::stringstream to_str;
        to_str << var_literal;
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
    };
};