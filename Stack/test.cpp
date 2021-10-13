#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "test.h"
#include "core.h"


void TestStackCtor() {
	printf("Testing StackCtor:\n\n");

	Stack st = {};

	printf("Testing for stack creation...\n");

	if ((StackCtor(&st) != 0)) {
		printf("Test failed: func does not create stack\n");
	}

	else {
		printf("Test successful\n\n");
	}

	printf("Testing for secondary stack creation...\n");

	if ((StackCtor(&st) != 1)) {
		printf("Test failed: func does not detect already detected stack\n");
	}

	else {
		printf("Test successful\n\n");
	}

	if (!(st.data)) {
		free(st.data);
	}
	printf("\n");
}

void TestPushPop(int n) {
	printf("Testing StackPush and StackPop using %d-sized arrays:\n\n", n);

	Stack st = {};
	int success = 1;
	
	printf("Testing for non-created stack detection...\n");

	if (!StackPush(&st, 1)) {
		printf("Test failed: StackPush does not detect non-created stack\n");
		success = 0;
	}
	
	int t = 0;

	if (!StackPop(&st, &t)) {
		printf("Test failed: StackPop does not detect non-created stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	if (StackCtor(&st)) {
		printf("Cannot create test stack\n");
		return;
	}

	printf("Testing StackPush...\n");

	if (StackPush(&st, 1)) {
		printf("Test failed: StackPush could not push element\n");
		success = 0;
	}

	else if (!st.data) {
		printf("NULL st.data pointer\n");
		return;
	}

	else if (*(st.data) != 1) {
		printf("Test failed: pushed value is wrong");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	printf("Testing StackPop...\n");

	int test = 0;

	if (StackPop(&st, &test)) {
		printf("Test failed: StackPop could not pop element\n");
		success = 0;
	}

	else if (test != 1) {
		printf("Test failed: popped value is wrong\n");
		success = 0;
	}

	else if (!StackPop(&st, &test)) {
		printf("Test failed: StackPop does not detect empty stack\n");
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	printf("Testing for stack operations priority...\n");

	int* a = (int*)calloc(n, sizeof(int));
	int* b = (int*)calloc(n, sizeof(int));

	if (!a) {
		printf("Memory allocation error\n");
		return;
	}

	if (!b) {
		printf("Memory allocation error\n");
		return;
	}

	for (int i = 0; i < n; i++) {
		a[i] = i + 1;
	}

	for (int i = 0; i < n; i++) {
		StackPush(&st, a[i]);
	}

	for (int i = 0; i < n; i++) {
		StackPop(&st, b + n - 1 - i);
	}

	for (int i = 0; i < n; i++) {
		if (a[i] != b[i]) {
			printf("Test failed: a[%d] = %d does not match with b[%d] = %d\n", i, a[i], i, b[i]);
			success = 0;
		}
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	free(st.data);
	printf("\n");
}

void TestStackDtor() {
	printf("Testing StackDtor:\n");

	Stack st = {};
	int success = 1;

	printf("Testing for destroying non-created stack...\n");

	if (!StackDtor(&st)) {
		printf("StackDtor does not detect non-created stack\n");
			success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	if (StackCtor(&st)) {
		printf("Cannot create test stack\n");
		return;
	}

	int* ptr = st.data;

	printf("Testing for destroying stack...\n");

	if (StackDtor(&st)) {
		printf("Test failed: StackDtor could not destroy stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	printf("Testing for detecting already destroyed stack...\n");

	if (!StackDtor(&st)) {
		printf("Test failed: StackDtor could not detected already destroyed stack\n");
		success = 0;
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	printf("Testing for poisoning stack elements...\n");

	for (int i = 0; i < st.capacity; i++) {
		if (*(ptr + i) != 0xBADBAD) {
			printf("Test failed: StackDtor did not poison %d element\n", i);
			printf("Element = %d, Poison = %d\n", *(ptr + i), 0xBADBAD);
			success = 0;
		}
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	if (!(st.data)) {
		free(st.data);
	}

	printf("\n");
}

void TestStackResize(int n) { //переделать
	printf("Testing hysteresis loop...\n");

	Stack st = {};
	int success = 1;

	if (StackCtor(&st)) {
		printf("Cannot create test stack\n");
		return;
	}

	int old_capacity = st.capacity;

	/*for (int i = 1; i < (int)log2((double)EXP_LIM) + 2; i++) {
		old_capacity = st.capacity;
		old_size = st.size;

		доделать

	}*/


	int i = 0; 

	for (i = 0; i < n; i++) {
		old_capacity = st.capacity;

		if (StackPush(&st, 1)) {
			printf("Test failed\n");
			success = 0;
			break;
		}
		if (old_capacity != st.capacity) {
			printf("New size = %d, new capacity = %d\n", st.size, st.capacity);
		}


	}

	for (; i > 0; i--) {
		int a = 0;
		old_capacity = st.capacity;
		if (StackPop(&st, &a)) {
			printf("Test failed\n");
			success = 0;
			break;
		}

		if (old_capacity != st.capacity) {
			printf("New size = %d, new capacity = %d\n", st.size, st.capacity);
		}
	}

	if (success) {
		printf("Test successfull\n\n");
	}

	if (!(st.data)) {
		free(st.data);
	}

	printf("\n");
}