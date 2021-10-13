#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "core.h"


int StackCtor(Stack* st) {
	assert(st);

	if (st->status) {
		printf("This stack was already created!\n");
		return 1;
	}

	st->capacity = MIN_CAP;
	st->size = 0;
	st->data = (int*)calloc(st->capacity, sizeof(int));

	if (!(st->data)) {
		printf("Cannot create stack\n");
		return 1;
	}

	st->status = 1;

	return 0;
}

int StackPush(Stack* st, int value) {
	assert(st);

	if (!st->status) {
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
	
	*(st->data+(st->size++)) = value;
	return 0;
}

int StackPop(Stack* st, int *value) {
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

	*value = *(st->data + (st->size-- - 1));

	int gap = st->capacity / LIN_ADD + 2;

	if ((st->size < st->capacity / 2 - gap) && st->size > MIN_CAP)  {
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

	for (int i = 0; i < st->capacity; i++) {
		st->data[i] = 0xBADBAD;
	}

	st->size = -1;
	st->capacity = -1;
	free(st->data);
	st->data = (int*)0xBADBAD;

	st->status = 0;

	return 0;
}

int StackResize(Stack* st, int param) {
	assert(st);
	assert(param != 0);

	int old_cap = st->capacity;
	//void* ptr = (void*)st->data;

	if (param > 0) {
		if (st->capacity < MIN_CAP * EXP_LIM) {
			st->data = (int*)realloc((void *)st->data, old_cap * sizeof(int) * 2);
			st->capacity = st->capacity * 2;
			//free(ptr);
		}

		else {
			st->data = (int*)realloc((void *)st->data, (old_cap + MIN_CAP * LIN_ADD) * sizeof(int));
			st->capacity = st->capacity + MIN_CAP * LIN_ADD;
		}
	}

	else {
		st->data = (int*)realloc((void*)st->data, (old_cap / 2) * sizeof(int));
		st->capacity = st->capacity / 2;

		/*for (int i = st->capacity; i < old_cap; i++) {
			*((int*)ptr + i) = 0xBADBAD;
		}*/
	}

	if (!st->data) {
		printf("Allocation error\n");
		return 1;
	}

	else {
		return 0;
	}


}