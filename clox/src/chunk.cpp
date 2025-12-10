#include <cstdlib>

#include "../headers/memory.hpp"
#include "../headers/chunk.hpp"

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->lines = NULL;
	chunk->code = NULL;
	initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

int length(int* arr) {
	int c = 0;

	while(arr != nullptr) {
		arr++;
		c++;
	}

	return c;
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
	if (chunk->capacity < chunk->count + 1) {
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
		// chunk->lines = GROW_ARRAY(int, chunk->lines,
		// oldCapacity, chunk->capacity);
	}

	int oldLineLength = length(chunk->lines);

	if (line > oldLineLength) {
		chunk->lines = GROW_ARRAY(int, chunk->lines, 
		oldLineLength, oldLineLength + 1);
	}

	chunk->code[chunk->count] = byte;
	// chunk->lines[chunk->count] = line;
	chunk->lines[line]++;
	chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}

void writeConstant(Chunk* chunk, Value value, int line) {
	writeValueArray(&chunk->constants, value);
	
	int oldLineLength = length(chunk->lines);

	if (line > oldLineLength) {
		chunk->lines = GROW_ARRAY(int, chunk->lines, 
		oldLineLength, oldLineLength + 1);
	}

}

int getLine(Chunk* chunk, int index) {
	int* line = chunk->lines;
	int instrCount = 0;
	int lineNo = 0;

	while(line != nullptr) {
		if (instrCount >= index) {
			return lineNo;
		}
		instrCount += *line;
		line++;
		lineNo++;
	}

	return -1;
}