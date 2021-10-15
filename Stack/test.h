#pragma once

//Tests StackCtor for creation and recreation
void TestStackCtor();

//Tests StackPush and StackPop by comparing two arrays of int n numbers
void TestPushPop(int n);


//Tests StackDtor for destroying, re-destroying stack and for poisoning stack elements
void TestStackDtor();

//Tests StackResize hysteresis loop by pushing to int n and popping back to MIN_CAP elements
void TestStackResize(int n);
