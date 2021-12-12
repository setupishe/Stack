#pragma once

#define StackCtor(St, a) TrueStackCtor(&St, a, #St)
#define StackCheck(St) TrueStackCheck(St, __FUNCSIG__, __FILE__, __LINE__)

#include "config.h"

//Stack stores int values in data; 
// current amount = size; 
// max amount = capacity; 
// initialized/unintialized = status; 
// item size in bytes = itype
// stack name = name
typedef struct {
    int size;
    int capacity;
    void* data;
    int status;
    size_t itype;
    char* name;
} Stack;

const int MIN_CAP = 100;
const int EXP_LIM = 100;
const int LIN_ADD = 10;

//Puts stname to st.name
int StackNamer(Stack *St, const char* stname);

//Creates stack for stack *st for items' size isize
int TrueStackCtor(Stack* st, size_t isize, const char* stack_name);

//Pushes int value to stack *st
int StackPush(Stack* st, const void* ptr);
//Pops last element from stack *st, puts it into int *value
int StackPop(Stack* st, void *value);

//Destroys stack *st, initializing its items with poison
int StackDtor(Stack* st);

//Increases or decreases stack *st depending on sign of int param
int StackResize(Stack* st, int param);

//Prints every stack element
int AllStackPrint(Stack* st, FILE* output);

//Returns 18-symbols line of current time and date
int GetTime(char* out);

//Prints every stack parameter
int StackDump(Stack* st);


//Checks Stack's integrity. Returns 1 for broken stack, 0 for normal stack
int TrueStackCheck(Stack* St, const char* funcname, const char* filename, int linename);
