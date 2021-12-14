#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "test.h"
#include "core.h"

typedef struct TortureChamberStruct{
	char LeftExec[1] = { 'a' };
	Stack Victim;
	char RightExec[1] = { 'a' };
}TortureChamber;

void TestStackNamer(const char * testname) {

	printf("\n\n-------------------------Testing StackNamer:-------------------------\n\n");

	Stack st = {};
	int success = 1;

	printf("Testing for zero length name...\n");
	{
		if (StackNamer(&st, "") != 1) {
			printf("Test failed: StackNamer could not detect zero length name\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	printf("Testing for non-zero length name...\n");
	{

		if (StackNamer(&st, testname)) {
			printf("Test failed: StackNamer could not name stack\n");
			success = 0;
		}

		else {

			int n = strlen(testname);

			for (int i = 0; i < n; i++) {

				if (st.name) {
					if (*(st.name + i) != *(testname + i)) {
						printf("Test failed: stack name does not match testname\n");
						success = 0;
					}
				}
			}
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	free(st.data);
}

void TestStackCtor() {
	printf("\n\n-------------------------Testing StackCtor:-------------------------\n\n");

	Stack st = {};

	printf("Testing for stack creation...\n");
	{
		if ((StackCtor(st, sizeof(int)) != 0)) {
			printf("Test failed: func does not create stack\n");
		}

		else {
			printf("Test successful\n\n");
		}
	}

	printf("Testing for secondary stack creation...\n");
	{
		if ((StackCtor(st, sizeof(int)) != 1)) {
			printf("Test failed: StackCtor does not detect already created stack\n");
		}

		else {
			printf("Test successful\n\n");
		}

		if (!(st.data)) {
			free(st.data);
		}
	}

	printf("Testing for various item types...\n");
	{
		for (size_t i = 1; i <= 64; i *= 2) {
			printf("Creating %d-bytes type stack:\n", i);
			Stack test_st = {};

			if (StackCtor(test_st, i)) {
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
	}

	printf("\n");
}

void TestPushPop(int n) {
	printf("\n\n-------------------------Testing StackPush and StackPop using %d-sized arrays:-------------------------\n\n", n);

	Stack st = {};
	int success = 1;
	
	printf("Testing for uninitialized stack detection...\n");

	{	
		int t = 0;
		
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
	}

	if (StackCtor(st, sizeof(stest))) {
		printf("Cannot create test stack\n");
		return;
	}


	printf("Testing push/pop operations...\n");
	{
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

	printf("Testing for stack operations priority...\n");
	{
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
	printf("\n\n-------------------------Testing StackDtor:-------------------------\n\n");

	Stack st = {};
	int success = 1;

	printf("Testing for destroying uninitialized stack...\n");
	{
		if (!StackDtor(&st)) {
			printf("StackDtor does not detect uninitialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}
	if (StackCtor(st, sizeof(stest))) {
		printf("Cannot create test stack\n");
		return;
	}

	printf("Testing for destroying stack...\n");
	{
		if (StackDtor(&st)) {
			printf("Test failed: StackDtor could not destroy stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	printf("Testing for detecting already destroyed stack...\n");
	{
		if (!StackDtor(&st)) {
			printf("Test failed: StackDtor could not detected already destroyed stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	if (!(st.data)) {
		free(st.data);
	}
}

void TestStackResize(int n) { 
	printf("\n\n-------------------------Testing StackResize-------------------------\n\n");

	Stack st = {};
	int success = 1;

	printf("Testing uninitialized stack detection...\n");
	{
		if (!StackResize(&st, 1)) {
			printf("Test failed: StackResize could not detect uninitialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	printf("Testing hysteresis loop...\n");
	{
		if (StackCtor(st, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		int old_capacity = st.capacity;

		int i = 0;
		stest t = { 0, 0, 0 };
		for (i = 0; i < n; i++) {
			old_capacity = st.capacity;
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
			old_capacity = st.capacity;
			if (StackPop(&st, &t)) {
				printf("Test failed: could not pop element\n");
				return;
			}

			if (old_capacity != st.capacity) {
				printf("New size = %d, new capacity = %d\n", st.size, st.capacity);
			}

		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	if (!(st.data)) {
		free(st.data);
	}

	printf("\n");
}

void TestStackDump() {
	printf("\n\n-------------------------Testing StackDump-------------------------\n\n");

	Stack st = {};
	int success = 1;

	printf("Testing for dumping uninitialized stack\n");
	{
		if (StackDump(&st) == 1) {
			printf("Test failed: StackDump could not dump uninitialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}
	}

	printf("Testing for dumping initialized stack\n");
	{
		if (StackCtor(st, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		stest t = { 0, 0, 0 };
		for (int i = 0; i < st.capacity / 2; i++) {
			if (StackPush(&st, &t)) {
				printf("Test failed: could not push element\n");
				return;
			}

			t.a++;
			t.b++;
			t.c++;
		}

		if (StackDump(&st) == 1) {
			printf("Test failed: StackDump could not print initialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successfull\n\n");
		}

		free(st.data);
	}


}

void TestStackCheck() {

	int success = 1;

	printf("\n\n-------------------------Testing StackCheck-------------------------\n\n");

	printf("Testing with non-initialized stack...\n");
	{
		Stack NCstack = {};

		if (StackCheck(&NCstack) != 0) {
			printf("Test failed: StackCheck could not recognise non-initialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successful\n");
		}
	}

	printf("Testing with not-broken initialized stack...\n");
	{
		Stack Cstack = {};
		
		if (StackCtor(Cstack, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		if (StackCheck(&Cstack) != 0) {
			printf("Test failed: StackCheck could not recognise not-broken initialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successful\n");
		}

		free(Cstack.data);
	}
	
	printf("Testing with not-broken destroyed initialized stack...\n");
	{
		Stack Dstack = {};

		if (StackCtor(Dstack, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		if (StackCheck(&Dstack) != 0) {
			printf("Test failed: StackCheck could not recognise not-broken destroyed stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successful\n");
		}
		free(Dstack.data);
	}

	printf("Testing with broken-status stack...\n");
	{
		Stack Bstack = {};
		Bstack.status = 8;

		if (StackCheck(&Bstack) == 0) {
			printf("Test failed: StackCheck could not recognise broken-status stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successful\n");
		}
		free(Bstack.data);
	}

	printf("Testing with broken initialized stack...\n");
	{
		Stack CBstack = {};
		if (StackCtor(CBstack, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		CBstack.size = -1;

		if (StackCheck(&CBstack) == 0) {
			printf("Test failed: StackCheck could not recognise broken initialized stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successful\n");
		}
		free(CBstack.data);
	}

	printf("Testing with broken destroyed stack...\n");
	{
		Stack DBstack = {};
		if (StackCtor(DBstack, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		StackDtor(&DBstack);

		DBstack.capacity = 2;

		if (StackCheck(&DBstack) == 0) {
			printf("Test failed: StackCheck could not recognise broken destroyed stack\n");
			success = 0;
		}

		if (success) {
			printf("Test successful\n");
		}
	}


}

void TestCanary() {

	printf("\n\n-------------------------Testing canary protection-------------------------\n\n");


	printf("Testing for left attack...\n");
	{
		TortureChamber Butcher;

		if (StackCtor(Butcher.Victim, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		for (int i = 0; i < 5; i++) {
			Butcher.LeftExec[i] = 'c';
		}

		if (StackCheck(&(Butcher.Victim)) != STK_BAD_LCAN) {
			printf("Test failed: StackCheck could not detect left attack\n");
		}
		else {
			printf("Test successful\n");
		}
	}

	printf("Testing for right attack...\n");
	{
		TortureChamber Butcher;

		if (StackCtor(Butcher.Victim, sizeof(stest))) {
			printf("Cannot create test stack\n");
			return;
		}

		for (int i = 0; i < 5; i++) {
			Butcher.RightExec[-i] = 'c';
		}

		if (StackCheck(&(Butcher.Victim)) != STK_BAD_RCAN) {
			printf("Test failed: StackCheck could not detect right attack\n");
		}
		else {
			printf("Test successful\n");
		}
	}
}

void TestPrintError() {

	printf("\n\n-------------------------Testing StackPrintError-------------------------\n\n");

	unsigned long int code = 0;

	printf("Testing for no errors code...\n");
	{
		if (StackPrintError(code) != 0) {
			printf("Test failed: StackPrintError could not proceed zero errors code\n");
		}
		else {
			printf("Test successful\n");
		}
	}

	printf("Testing for all errors code...\n");
	{
		code = 511;

		if (StackPrintError(code) != 9) {
			printf("Test failed: StackPrintError could not proceed all errors code\n");
		}
		else {
			printf("Test successful\n");
		}
	}

	printf("Testing for all errors code...\n");
	{
		code = 441;

		if (StackPrintError(code) != 6) {
			printf("Test failed: StackPrintError could not proceed three errors code\n");
		}
		else {
			printf("Test successful\n");
		}
	}
}

void TestStackHash() {

	printf("\n\n-------------------------Testing StackPrintError-------------------------\n\n");

	printf("Testing with not-initialised stack...\n"); 
	{
		Stack st;

		if (StackHash(&st) != 0) {
			printf("Test failed: StackHash returned wrong HashSum\n");
		}

		else {
			printf("Test successful\n");
		}
	}
	

}