#pragma once


typedef struct {
	int a;
	double b;
	long double c;
}stest;

const stest POISON = { 1, 1, 1 };

//DEFINED BY USER: prints stack &st stack element to FILE* output, must return 0 for success or 1 for failure
int StackPrint(void *ptr, FILE* output);