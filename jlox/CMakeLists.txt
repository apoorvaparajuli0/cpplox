#have discontinued support for the AST Printer and RPN Printer

add_library(jlox
            "./src/Lox.cpp"         "./headers/Lox.hpp"
            "./src/Scanner.cpp"     "./headers/Scanner.hpp"
            "./src/Token.cpp"       "./headers/Token.hpp"
            "./src/Expr.cpp"        "./headers/Expr.hpp"
            "./src/Stmt.cpp"        "./headers/Stmt.hpp"
            "./src/Parser.cpp"      "./headers/Parser.hpp"
            "./src/Interpreter.cpp" "./headers/Interpreter.hpp"
            "./src/Environment.cpp" "./headers/Environment.hpp"
            "./src/Resolver.cpp"    "./headers/Resolver.hpp"
            "./src/LoxFunction.cpp" "./headers/LoxFunction.hpp"
            "./src/LoxClass.cpp"    "./headers/LoxClass.hpp"

            "./headers/ReturnError.hpp"  "./headers/RuntimeError.hpp" 
            "./headers/LoxCallable.hpp"  "./headers/TokenType.hpp"    
            "./headers/Visitor.hpp"
            )