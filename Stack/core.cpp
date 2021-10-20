#define _CRT_SECURE_NO_WARNINGS


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

int StackNamer(Stack* st, const char* stname) {
	assert(st);
	assert(stname);

	int n = strlen(stname);

	if (n == 0) {
		printf("Stname length = 0\n");
		return 1;
	}

	if (!(st->name = (char*)calloc(n, sizeof(char)))) {
		printf("Memory allocation error\n");
		return 1;
	}

	if (!(st->name = strcpy(st->name, stname))) {
		printf("String copying failed\n");
		return 1;
	}
	return 0;
}

int StackCtor(Stack* st, size_t isize) {
	assert(st);
	assert(isize > 0);

	if (st->status) {
		printf("Stack was already created\n");
		return 1;
	}

	st->capacity = MIN_CAP;
	st->size = 0;
	st->itype = isize;
	st->data = calloc(st->capacity, isize);

	if (!(st->data)) {
		printf("Cannot create stack\n");
		return 1;
	}

	st->status = 1;

	return 0;
}

int StackPush(Stack* st, const void *ptr) {
	assert(st);
	assert(ptr);

	if (!st->status) { // ToDo: assert function
		printf("Stack was not created\n");
		return 1;
	}

	if (st->size == st->capacity) {
		printf("Stack fully loaded. Current capacity = %d, current size = %d\n", st->capacity, st->size);
		printf("Resizing...\n");
		if (StackResize(st, 1)) {
			printf("Cannot resize stack\n");
			return 1;
		}
	}

	memcpy(((char*)st->data + (st->size++) * st->itype), ptr, st->itype);

	return 0;
}

int StackPop(Stack* st, void *value) {
	assert(st);
	assert(value);

	if (!st->status) {
		printf("Stack was not created\n");
		return 1;
	}

	if (st->size == 0) {
		printf("Cannot pop element from stack: stack is empty\n");
		return 1;
	}

	memcpy(value, (const void*)((char*)st->data + (st->size-- - 1) * st->itype), st->itype);


	int gap = st->capacity / LIN_ADD + 2;

	if ((st->size < st->capacity / 2 - gap) && st->size > MIN_CAP) {
		printf("Using less then half of stack's capacity. Current capacity = %d, current size = %d\n", st->capacity, st->size);
		printf("Resizing...\n");

		if (StackResize(st, -1)) {
			printf("Cannot resize stack\n");
			return 1;
		}
	}

	return 0;
}

int StackDtor(Stack* st) {
	assert(st);

	if (!st->status) {
		printf("Stack was already destroyed or was not created at all\n");
		return 1;
	}


	st->size = -1;
	st->capacity = -1;

	free(st->data);

	st->data = (int*)0xBADBAD; //todo: users poison
	st->status = 0;

	return 0;
}

int StackResize(Stack* st, int param) {
	assert(st);
	assert(param != 0);

	if (st->status == 0) {
		printf("Stack was already destroyed or was not created at all\n");
		return 1;
	}

	int old_cap = st->capacity;
	void* ptr = st->data;

	if (param > 0) {
		if (st->capacity < MIN_CAP * EXP_LIM) {
			st->data = realloc(ptr, st->capacity * st->itype * 2); 
			st->capacity = st->capacity * 2;
		}

		else {
			st->data = realloc(ptr, (st->capacity + MIN_CAP * LIN_ADD) * st->itype);
			st->capacity = st->capacity + MIN_CAP * LIN_ADD;
		}
	}

	else {
		st->data = realloc(ptr, (st->capacity / 2) * st->itype);
		st->capacity = st->capacity / 2;
	}


	if (!st->data) {
		printf("Allocation error\n");
		st->data = ptr;
		return 1;
	}

	else if (param > 0) {
		for (int i = old_cap; i < st->capacity; i++) {
			memcpy(((char*)st->data + i * st->itype), &POISON, st->itype);
		}
	}
	
	return 0;
}
