add_library(src
            Lox.cpp         "../headers/Lox.hpp"
            Scanner.cpp     "../headers/Scanner.hpp"
            Token.cpp       "../headers/Token.hpp"
            Expr.cpp        "../headers/Expr.hpp"
            AstPrinter.cpp  "../headers/AstPrinter.hpp"
            RPNPrinter.cpp  "../headers/RPNPrinter.hpp"
            Parser.cpp      "../headers/Parser.hpp"
            Interpreter.cpp "../headers/Interpreter.hpp"
            
            "../headers/TokenType.hpp" "../headers/Visitor.hpp"
            )