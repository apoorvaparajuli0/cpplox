#include "./jlox/headers/Lox.hpp"
#include "./clox/headers/common.hpp"
#include "./clox/headers/chunk.hpp"
#include "./clox/headers/debug.hpp"

int main(int argc, char** argv){
    if(argc > 3 || std::string(argv[0]) != "./cpplox" || argc < 2) {
        printf("%s\n", "Usage: cpplox -[implementation (e.g. clox, jlox)] [script]");
        exit(-1);
    } 
    if(argc == 3) {
        if(std::string(argv[1]) == "-jlox") {
            Lox::runFile(argv[2]);
        } else if(std::string(argv[1]) == "-clox") {
            exit(-1);
        }
    } else if(argc == 2) {
        if(std::string(argv[1]) == "-jlox") {
            Lox::runPrompt();
        } else if(std::string(argv[1]) == "-clox") {
            //run clox prompt
            Chunk chunk;
            initChunk(&chunk);
            int constant = addConstant(&chunk, 1.2);
            writeChunk(&chunk, OP_CONSTANT, 123);
            writeChunk(&chunk, constant, 123);
            writeChunk(&chunk, OP_RETURN, 123);
            disassembleChunk(&chunk, "test chunk");
            freeChunk(&chunk);
        }
    } 

    return 0;
}
