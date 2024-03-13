#include "Lox.hpp"

int main(int argc, char** argv){
    if(argc > 1) {
        printf("%s", "Usage: cpplox [script]");
        exit(64);
    } else if(argc == 1) {
        Lox::runFile(argv[0]);
    } else {
        Lox::runPrompt();
    }
}
