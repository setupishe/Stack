#pragma once

#define StackCtor(St, a) TrueStackCtor(&St, a, #St)
#define StackCheck(St) TrueStackCheck(St, __FUNCSIG__, __FILE__, __LINE__)
#define ASSERT(a); if (a != 0) {printf("\nStack's integrity compromised: exiting... \n");\
    unsigned long int ErrorCode = a; StackPrintError(ErrorCode);\
    exit(ErrorCode);}

#include "config.h"

typedef unsigned long int canary;
static canary LCAN_VALUE = 0xBEBEBEBE;
static canary RCAN_VALUE = 0xBABABABA;
static canary D_LCAN_VALUE = 0xDADADADA;
static canary D_RCAN_VALUE = 0xDEDEDEDE;
static canary N_LCAN_VALUE = 0xCACACACA;
static canary N_RCAN_VALUE = 0xCECECECE;


//Stack stores int values in data; 
// current amount = size; 
// max amount = capacity; 
// initialized/unintialized = status; 
// item size in bytes = itype
// stack name = name
// "canary" guards: LCan and RCan for stack structure values
// pointers for "canary" guards: DataLCanPtr and DataRCanPtr for data integrity, NameLCanPtr and NameRCanPtr for name integrity,

typedef struct StackStruct{
    canary LCan = LCAN_VALUE;

    int size = 0;
    int capacity = 0;
    int status = 0;
    size_t itype = 0;

    void* datachunk = NULL;
    void* data = NULL;
    canary* DataLCanPtr = NULL;
    canary* DataRCanPtr = NULL;

    void* namechunk = NULL;
    char* name = NULL;
    canary* NameLCanPtr = NULL;
    canary* NameRCanPtr = NULL;
    unsigned long int HashSum = 0;

    canary RCan = RCAN_VALUE;

} Stack;

enum Sizes {
    STK_MIN_CAP = 100,
    STK_EXP_LIM = 100,
    STK_LIN_ADD = 10
};

enum statuses {
    STK_NOT_INITIALISED = 0,
    STK_INITIALISED = 1,
    STK_DESTROYED = -1
};

enum Errors : unsigned long {
    STK_BAD_SIZE = 1,
    STK_BAD_CAP = 2,
    STK_BAD_ITYPE = 4,
    STK_OVERFLOW = 8,
    STK_BAD_DATA_PTR = 16,
    STK_BAD_STATUS = 32,
    STK_BAD_NAME = 64,
    STK_BAD_LCAN = 128,
    STK_BAD_RCAN = 256,
    STK_BAD_D_LCAN = 512,
    STK_BAD_D_RCAN = 1024,
    STK_BAD_N_LCAN = 2048,
    STK_BAD_N_RCAN = 4096
};

static const char* ErrorNames[] = {
    "ERROR: Stack's size is not valid\n",
    "ERROR: Stack's capacity is not valid\n",
    "ERROR: Stack's item type is not valid\n",
    "ERROR: Stack overflow! Size exceeds capacity\n",
    "ERROR: Stack's data pointer is not valid\n",
    "ERROR: Stack's status is not valid\n",
    "ERROR: Stack's name is not valid\n",
    "ERROR: Stack's left main canary is not valid. We are being attacked!\n",
    "ERROR: Stack's right main canary is not valid. We are being attacked!\n",
    "ERROR: Stack's left data canary is not valid. We are being attacked!\n",
    "ERROR: Stack's right data canary is not valid. We are being attacked!\n",
    "ERROR: Stack's left name canary is not valid. We are being attacked!\n",
    "ERROR: Stack's right name canary is not valid. We are being attacked!\n",
};

//Puts stname to st.name
int StackNamer(Stack *St, const char* stname);

//Creates stack for stack *st for items' size isize
int TrueStackCtor(Stack* st, size_t isize, const char* STK_name);

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

//Checks Stack's integrity. Returns 0 for normal stack and error(s) value for broken stack
unsigned long int TrueStackCheck(Stack* St, const char* funcname, const char* filename, int linename);

//Prints error(s) names using unsigned long int error parameter. Returns number of printed errors or -1 in case of failure
int StackPrintError(unsigned long int error);

//Makes hash-sum of Stack and puts it into it's HashSum value
unsigned long int StackHash(Stack* st);

//Assigns data and datacanaries pointers after datachunk (re)allocation
int StackArrangeData(Stack* st);