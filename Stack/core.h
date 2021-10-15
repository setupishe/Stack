#pragma once



//Stack stores int values in data; 
// current amount = size; 
// max amount = capacity; 
// initialized/unintialized = status; 
// item size in bytes = itype
typedef struct {
    int size;
    int capacity;
    void* data;
    int status;
    size_t itype;
} Stack;

const int MIN_CAP = 100;
const int EXP_LIM = 100;
const int LIN_ADD = 10;

//Creates stack for stack *st for items' size isize
int StackCtor(Stack* st, size_t isize);

//Pushes int value to stack *st
int StackPush(Stack* st, const void* ptr);
//Pops last element from stack *st, puts it into int *value
int StackPop(Stack* st, void *value);

//Destroys stack *st, initializing its items with poison
int StackDtor(Stack* st);

//Increases or decreases stack *st depending on sign of int param
int StackResize(Stack* st, int param);


