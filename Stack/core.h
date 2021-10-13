#pragma once

//Stack stores int values in data, current amount = size, max amount = capacity
typedef struct {
    int size;
    int capacity;
    int* data;
    int status;
} Stack;

const int MIN_CAP = 100;
const int EXP_LIM = 100;
const int LIN_ADD = 10;

//Creates stack for stack *st
int StackCtor(Stack* st);

//Pushes int value to stack *st
int StackPush(Stack* st, int value);

//Pops last element from stack *st, puts it into int *value
int StackPop(Stack* st, int *value);

//Destroys stack *st, initializing its items with poison
int StackDtor(Stack* st);

//Increases or decreases stack *st depending on sign of int param
int StackResize(Stack* st, int param);


