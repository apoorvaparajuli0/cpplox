#include "./jlox/headers/Lox.hpp"

int main(int argc, char** argv){
    if(argc > 3) {
        printf("%s\n", "Usage: cpplox -[implementation (e.g. clox, jlox)] [script]");
        exit(64);
    } else if(argc == 3) {
        if(std::string(argv[1]) == "-jlox") {
            Lox::runFile(argv[2]);
        } else if(std::string(argv[1]) == "-clox") {
            //run clox file
        }
    } else if(argc == 2) {
        if(std::string(argv[1]) == "-jlox") {
            Lox::runPrompt();
        } else if(std::string(argv[1]) == "-clox") {
            //run clox prompt
        }
    } 
    //Leave this while developing clox
    else if(argc == 1) {
        //run clox prompt
    }

    return 0;
}
