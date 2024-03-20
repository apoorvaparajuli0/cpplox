#include "../headers/Lox.hpp"
#include "../headers/AstPrinter.hpp"

int main(int argc, char** argv){
    if(argc > 2) {
        printf("%s", "Usage: cpplox [script]");
        exit(64);
    } else if(argc == 2) {
        std::string cmd = "ast_printer";
        if(argv[1] == cmd) {
            Object literal = std::nullptr_t{};
            std::string op_lexeme = "-";

            expr_ptr lit1(new Literal(123.0));
            expr_ptr lit2(new Literal(45.67));

            Token op1 = Token(TokenType::MINUS, op_lexeme, literal, 1);
            op_lexeme = "*";
            Token op2 = Token(TokenType::STAR, op_lexeme, literal, 1);

            expr_ptr grp(new Grouping(std::move(lit2)));

            expr_ptr unary(new Unary(op1, std::move(lit1)));

            expr_ptr expr(new Binary(std::move(unary), op2, std::move(grp)));

            std::initializer_list<expr_ptr> exprs = {std::move(expr)};
            AstPrinter printer;

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
