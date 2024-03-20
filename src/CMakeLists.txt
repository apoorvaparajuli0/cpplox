add_library(src
            Lox.cpp Lox.hpp
            Scanner.cpp Scanner.hpp
            Token.cpp Token.hpp
            Expr.cpp Expr.hpp
            AstPrinter.cpp AstPrinter.hpp
            Visitor.hpp
            TokenType.hpp)