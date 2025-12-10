#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.hpp"
#include "value.hpp"

typedef enum {
	OP_CONSTANT,
	OP_CONSTANT_LONG,
	OP_RETURN,
} OpCode;

typedef struct {  
	int count;
	int capacity;
	uint8_t* code;
	int* lines;
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
int getLine(Chunk* chunk, int index);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
int length(int* arr);
void writeConstant(Chunk* chunk, Value value, int line);

#endif