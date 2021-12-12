#pragma once

#define StackCtor(St, a) TrueStackCtor(&St, a, #St)
#define StackCheck(St) TrueStackCheck(St, __FUNCSIG__, __FILE__, __LINE__)
#define ASSERT(a); if (a != 0) {printf(ErrorNames[a - 1]); exit(a);}

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

enum Sizes {
    STACK_MIN_CAP = 100,
    STACK_EXP_LIM = 100,
    STACK_LIN_ADD = 10
};

enum statuses {
    STACK_NOT_INITIALISED = 0,
    STACK_INITIALISED = 1,
    STACK_DESTROYED = -1
};

enum Errors {
    STACK_BAD_SIZE = 1, STACK_BAD_CAP, STACK_BAD_ITYPE, STACK_OVERFLOW, STACK_BAD_DATA_PTR, STACK_BAD_STATUS, STACK_BAD_NAME
};

static const char* ErrorNames[] = { //œŒ◊≈Ã” —“¿“» ?
    "ERROR: Stack's size is not valid\n",
    "ERROR: Stack's capacity is not valid\n",
    "ERROR: Stack's item type is not valid\n",
    "ERROR: Stack overflow! Size exceeds capacity\n",
    "ERROR: Stack's data pointer is not valid\n",
    "ERROR: Stack's status is not valid\n",
    "ERROR: Stack's name is not valid\n",
};

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


//Checks Stack's integrity. Returns 0 for normal stack and error value for broken stack
int TrueStackCheck(Stack* St, const char* funcname, const char* filename, int linename);
