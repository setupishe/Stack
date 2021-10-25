#pragma once
#include "config.h"

//Tests StackNamer for zero length name and non-zero name char *testname
void TestStackNamer(const char* testname);

//Tests TrueStackCtor for creation and recreation
void TestStackCtor();

//Tests StackPush and StackPop by comparing two arrays of int n numbers
void TestPushPop(int n);

//Tests StackDtor for destroying, re-destroying stack and for poisoning stack elements
void TestStackDtor();

//Tests StackResize hysteresis loop by pushing to int n and popping back to MIN_CAP elements
void TestStackResize(int n);

//Tests StackDump for initialized and uninitialized stack
void TestStackDump();

//Tests StackCheck for broken and not-broken stack
void TestStackCheck();
