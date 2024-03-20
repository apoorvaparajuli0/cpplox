#include "../headers/Lox.hpp"
#include "../headers/AstPrinter.hpp"
#include "../headers/RPNPrinter.hpp"

int main(int argc, char** argv){
    if(argc > 2) {
        printf("%s", "Usage: cpplox [script]");
        exit(64);
    } else if(argc == 2) {
        std::string cmd = argv[1];

        Object literal = std::nullptr_t{};

        expr_ptr lit1(new Literal(1.0));
        expr_ptr lit2(new Literal(2.0));
        expr_ptr lit3(new Literal(4.0));
        expr_ptr lit4(new Literal(3.0));

        std::string op_lexeme = "-";
        Token op1 = Token(TokenType::MINUS, op_lexeme, literal, 1);
        op_lexeme = "*";
        Token op2 = Token(TokenType::STAR, op_lexeme, literal, 1);
        op_lexeme = "+";
        Token op3 = Token(TokenType::PLUS, op_lexeme, literal, 1);

        expr_ptr expr1(new Binary(std::move(lit1), op3, std::move(lit2)));
        expr_ptr expr2(new Binary(std::move(lit3), op1, std::move(lit4)));

        expr_ptr grp1(new Grouping(std::move(expr1)));
        expr_ptr grp2(new Grouping(std::move(expr2)));

        expr_ptr expr(new Binary(std::move(grp1), op2, std::move(grp2)));

        std::initializer_list<expr_ptr> exprs = {std::move(expr)};

        if(cmd == "ast_printer") {
            AstPrinter printer;
            printf("%s\n", printer.print(exprs).c_str());
            return 0;
        } else if(cmd == "rpn_printer") {
            RPNPrinter printer;
            printf("%s\n", printer.print(exprs).c_str());
            return 0;
        } else {
            Lox::runFile(argv[1]);
        }
    } else {
        Lox::runPrompt();
    }

    return 0;
}
