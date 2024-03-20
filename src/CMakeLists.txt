add_library(src
            Lox.cpp        "../headers/Lox.hpp"
            Scanner.cpp    "../headers/Scanner.hpp"
            Token.cpp      "../headers/Token.hpp"
            Expr.cpp       "../headers/Expr.hpp"
            AstPrinter.cpp "../headers/AstPrinter.hpp"
            "../headers/TokenType.hpp" "../headers/Visitor.hpp"
            )