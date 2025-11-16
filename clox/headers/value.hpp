#ifndef clox_value_h
#define clox_value_h

#include "common.hpp"

typedef double Value;

/*
 a constant pool for values that our virtual machine will use. Each array
 is associated with a bytecode chunk
*/
typedef struct {
	int capacity;
	int count;
	Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif