#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "test.h"
#include "core.h"

int AllStackPrint(Stack* st);


void TestStackNamer(const char * testname) {

	printf("-------------------------Testing StackNamer:-------------------------\n\n");

	Stack st = {};
	int success = 1;

	{
		printf("Testing for zero length name...\n");

		if (StackNamer(&st, "") != 1) {
			printf("Test failed: StackNamer could not detect zero length name\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}
	{
		printf("Testing for non-zero length name...\n");

		if (StackNamer(&st, testname)) {
			printf("Test failed: StackNamer could not name stack\n");
			success = 0;
		}

		else {

			int n = strlen(testname);

			for (int i = 0; i < n; i++) {

				if (st.name) {
					if (*(st.name + i) != *(testname + i)) {
						printf("Test failed: stack name does not match testname");
						success = 0;
					}
				}
			}
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}
}

void TestStackCtor() {
	printf("-------------------------Testing StackCtor:-------------------------\n\n");

	Stack st = {};

	printf("Testing for stack creation...\n");
	
	if ((StackCtor(&st, sizeof(int)) != 0)) {
		printf("Test failed: func does not create stack\n");
	}

	else {
		printf("Test successful\n\n");
	}

	printf("Testing for secondary stack creation...\n");

	if ((StackCtor(&st, sizeof(int)) != 1)) {
		printf("Test failed: func does not detect already created stack\n");
	}

	else {
		printf("Test successful\n\n");
	}

	if (!(st.data)) {
		free(st.data);
	}

	printf("Testing for various item types...\n");

	for (size_t i = 1; i <= 64; i *= 2) {
		printf("Creating %d-bytes type stack:\n", i);
		Stack test_st = {};

		if (StackCtor(&test_st, i)) {
			printf("Test failed: StackCtor could not create stack\n");
		}

		else if (test_st.itype != i) {
			printf("Test failed: stack.itype != i: %d != %d\n", test_st.itype, i);
		}

		else {
			printf("Test successfull\n");
		}

		if (!(test_st.data)) {
			free(test_st.data);
		}
	}

	printf("\n");
}

void TestPushPop(int n) {
	printf("-------------------------Testing StackPush and StackPop using %d-sized arrays:-------------------------\n\n", n);

	Stack st = {};
	int success = 1;
	
	{	int t = 0;
		printf("Testing for uninitialized stack detection...\n");
		
		if (!StackPush(&st, &t)) {
			printf("Test failed: StackPush does not detect uninitialized stack\n");
			success = 0;
		}

		if (!StackPop(&st, &t)) {
			printf("Test failed: StackPop does not detect uninitialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}

		if (StackCtor(&st, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}
	}

	{
		printf("Testing push/pop operations...\n");

		success = 1;
		stest t1 = { 1, 2.0, 3 };

		if (StackPush(&st, &t1)) {
			printf("Test failed: StackPush could not push element\n");
			success = 0;
		}

		else if (!st.data) {
			printf("NULL st.data pointer\n");
			return;
		}

		stest t2 = { 0, 0, 0 };

		if (StackPop(&st, &t2)) {
			printf("Test failed: StackPop could not pop element\n");
			success = 0;
		}

		else if (t1.a != t2.a || t1.b != t2.b || t1.c != t2.c) {
			printf("Test failed: popped value does not match pushed value:\n");
			printf("Pushed value: %d %f %f\n", t1.a, t1.b, t1.c);
			printf("Popped value: %d %f %f\n", t2.a, t2.b, t2.c);
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}

		else if (!StackPop(&st, &t1)) {
			printf("Test failed: StackPop does not detect empty stack\n");
		}

	}

	{
		printf("Testing for stack operations priority...\n");

		success = 1;
		stest t = { 1, 2, 3};
		stest* one = (stest*)calloc(n, sizeof(stest));
		stest* two = (stest*)calloc(n, sizeof(stest));

		if (!one) {
			printf("Memory allocation error\n");
			return;
		}

		if (!two) {
			printf("Memory allocation error\n");
			return;
		}

		for (int i = 0; i < n; i++) {
			one[i].a++;
			one[i].b++;
			one[i].c++;
		}

		for (int i = 0; i < n; i++) {
			StackPush(&st, one + i);
		}

		for (int i = 0; i < n; i++) {
			StackPop(&st, two + n - 1 - i);
		}

		for (int i = 0; i < n; i++) {
			if (one[i].a != two[i].a || one[i].b != two[i].b || one[i].c != two[i].c) {
				printf("Test failed: popped value does not match pushed value:\n");
				printf("Pushed value: %d %f %f\n", one[i].a, one[i].b, one[i].c);
				printf("Popped value: %d %f %f\n", two[i].a, two[i].b, two[i].c);
				success = 0;
			}
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}
	free(st.data);
	printf("\n");
}
	
void TestStackDtor() {
	printf("-------------------------Testing StackDtor:-------------------------\n");

	Stack st = {};
	int success = 1;

	printf("Testing for destroying uninitialized stack...\n");

	if (!StackDtor(&st)) {
		printf("StackDtor does not detect uninitialized stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	success = 1;

	if (StackCtor(&st, sizeof(stest))) {
		printf("Cannot create test stack\n");
		return;
	}

	printf("Testing for destroying stack...\n");

	if (StackDtor(&st)) {
		printf("Test failed: StackDtor could not destroy stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	success = 1;

	printf("Testing for detecting already destroyed stack...\n");

	if (!StackDtor(&st)) {
		printf("Test failed: StackDtor could not detected already destroyed stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	if (!(st.data)) {
		free(st.data);
	}
}

void TestStackResize(int n) { 
	printf("-------------------------Testing StackResize-------------------------\n\n");

	Stack st = {};
	int success = 1;

	printf("Testing uninitialized stack detection...\n");

	if (!StackResize(&st, 1)) {
		printf("Test failed: StackResize could not detect uninitialized stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	printf("Testing hysteresis loop...\n");

	success = 1;

	if (StackCtor(&st, sizeof(stest))) {
		printf("Cannot create test stack\n");
		return;
	}

	int old_capacity = st.capacity;

	int i = 0; 

	for (i = 0; i < n; i++) {
		old_capacity = st.capacity;
		stest t = { 0, 0, 0 };
		if (StackPush(&st, &t)) {
			printf("Test failed: could not push element\n");
			return;
		}
		if (old_capacity != st.capacity) {
			printf("New size = %d, new capacity = %d\n", st.size, st.capacity);

			for (int k = st.size + 1; k < st.capacity; k++) {
				stest one = {};
				one = *(stest*)((char*)st.data + k * st.itype);
				if (one.a != POISON.a || one.b != POISON.b || one.c != POISON.c) {
					printf("Test failed: StackResize did not poison %d element\n", k);
					success = 0;
				}
			}

			if (success) {
				printf("Poisoning successfull\n\n");
			}
		}

		t.a++;
		t.b++;
		t.c++;
	}

	for (; i > 0; i--) {
		stest t = { 0, 0, 0 };
		old_capacity = st.capacity;
		if (StackPop(&st, &t)) {
			printf("Test failed: could not pop element\n");
			return;
		}

		if (old_capacity != st.capacity) {
			printf("New size = %d, new capacity = %d\n", st.size, st.capacity);
		}

		t.a++;
		t.b++;
		t.c++;

	}

	if (success) {
		printf("Test successfull\n\n");
	}

	if (!(st.data)) {
		free(st.data);
	}

	printf("\n");
}
int AllStackPrint(Stack* st) {

	printf("Printing stack...\n");

	for (int i = 0; i < st->capacity; i++) {

		printf("%d element:", i + 1);

		if (StackPrint((void*)((char*)st->data + i * st->itype))) {
			printf("StackPrint error\n");
			return 1;
		}
	}

	return 0;
}