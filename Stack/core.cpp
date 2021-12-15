#define _CRT_SECURE_NO_WARNINGS


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "core.h"

int StackNamer(Stack* st, const char* stname) {
	assert(st);
	assert(stname);

	int n = strlen(stname);

	if (n == 0) {
		printf("Stname length = 0\n");
		return 1;
	}

	if (!(st->namechunk = malloc(n*sizeof(char) + 2 * sizeof(canary) + 1))) {
		printf("Memory allocation error\n");
		return 1;
	}

	st->name = (char*)st->namechunk + sizeof(canary);
	st->NameLCanPtr = (canary*)st->namechunk;
	st->NameRCanPtr = (canary*)((char *)st->namechunk + n * sizeof(char) + sizeof(canary) + 1);

	*(st->NameLCanPtr) = N_LCAN_VALUE;
	*(st->NameRCanPtr) = N_RCAN_VALUE;


	if (!(st->name = strcpy(st->name, stname))) {
		printf("String copying failed\n");
		return 1;
	}
	else {
		return 0;
	}
}

int TrueStackCtor(Stack* st, size_t isize, const char* STK_name) {
	assert(st);
	assert(isize > 0);

	if (st->status == STK_INITIALISED) {
		printf("Stack was already created\n");
		return 1;
	}

	if (StackNamer(st, STK_name) == 1) {
		printf("StackNamer could not name stack\n");
		return 1;
	}

	st->capacity = STK_MIN_CAP;
	st->size = 0;
	st->itype = isize;

	if (!(st->datachunk = malloc(st->capacity * isize + 2 * sizeof(canary)))) {
		printf("Memory allocation error\n");
		return 1;
	}

	if (StackArrangeData(st) == 1) {
		printf("Data canaries arrangement error\n");
		return 1;
	}

	*(st->DataLCanPtr) = D_LCAN_VALUE;
	*(st->DataRCanPtr) = D_RCAN_VALUE;


	if (!(st->data)) {
		printf("Cannot create stack\n");
		return 1;
	}

	st->status = STK_INITIALISED;

#ifdef USEHASH
	if (StackHash(st) != 0) {
		printf("StackHash error\n");
		return 1;
	}
#endif

	return 0;
}

int StackPush(Stack* st, const void *ptr) {
	ASSERT(StackCheck(st));
	assert(ptr);

	if (st->status == STK_NOT_INITIALISED || st->status == STK_DESTROYED) {
		printf("Stack was destroyed or was not created at all\n");
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

#ifdef USEHASH
	if (StackHash(st) != 0) {
		printf("StackHash error\n");
		return 1;
	}
#endif

	return 0;
}

int StackPop(Stack* st, void *value) {
	assert(!StackCheck(st));
	assert(value);

	if (st->status == STK_NOT_INITIALISED || st->status == STK_DESTROYED) {
		printf("Stack was destroyed or was not created at all\n");
		return 1;
	}

	if (st->size == 0) {
		printf("Cannot pop element from stack: stack is empty\n");
		return 1;
	}

	memcpy(value, (const void*)((char*)st->data + (st->size-- - 1) * st->itype), st->itype);

#ifdef USEHASH
	if (StackHash(st) != 0) {
		printf("StackHash error\n");
		return 1;
	}
#endif

	int gap = st->capacity / STK_LIN_ADD + 2;

	if ((st->size < st->capacity / 2 - gap) && st->size > STK_MIN_CAP) {
		printf("Using less then a half of stack's capacity. Current capacity = %d, current size = %d\n", st->capacity, st->size);
		printf("Resizing...\n");

		if (StackResize(st, -1)) {
			printf("Cannot resize stack\n");
			return 1;
		}
	}

	return 0;
}

int StackDtor(Stack* st) {
	ASSERT(StackCheck(st));

	if (st->status == STK_NOT_INITIALISED || st->status == STK_DESTROYED) {
		printf("Stack was already destroyed or was not created at all\n");
		return 1;
	}


	st->size = -1;
	st->capacity = -1;
	st->itype = 0;
	free(st->datachunk);

	st->data = (int*)0xBADBAD; 
	st->status = STK_DESTROYED;

	return 0;
}

int StackResize(Stack* st, int param) {
	ASSERT(StackCheck(st));
	assert(param != 0);

	if (st->status == STK_NOT_INITIALISED) {
		printf("Stack was already destroyed or was not created at all\n");
		return 1;
	}

	int old_cap = st->capacity;
	void* ptr = st->datachunk;

	if (param > 0) {
		if (st->capacity < STK_MIN_CAP * STK_EXP_LIM) {
			canary temp = *st->DataRCanPtr;
			st->datachunk = realloc(ptr, st->capacity * st->itype * 2 + 2 * sizeof(canary)); 
			st->capacity = st->capacity * 2;

			if (StackArrangeData(st) == 1) {
				printf("Data canaries arrangement error\n");
				return 1;
			}

			*st->DataRCanPtr = temp;
		}

		else {
			canary temp = *st->DataRCanPtr;
			st->datachunk = realloc(ptr, (st->capacity + STK_MIN_CAP * STK_LIN_ADD) * st->itype + 2 * sizeof(canary));
			st->capacity = st->capacity + STK_MIN_CAP * STK_LIN_ADD;

			if (StackArrangeData(st) == 1) {
				printf("Data canaries arrangement error\n");
				return 1;
			}

			*st->DataRCanPtr = temp;
		}
	}

	else {
		canary temp = *st->DataRCanPtr;
		st->data = realloc(ptr, (st->capacity / 2) * st->itype);
		st->capacity = st->capacity / 2;

		if (StackArrangeData(st) == 1) {
			printf("Data canaries arrangement error\n");
			return 1;
		}

		*st->DataRCanPtr = temp;
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
	
#ifdef USEHASH
	if (StackHash(st) != 0) {
		printf("StackHash error\n");
		return 1;
	}
#endif

	return 0;
}

int AllStackPrint(Stack* st, FILE* output) {
	assert(output);
	fprintf(output, "Printing stack...\n");
	int max = st->capacity > STK_MIN_CAP ? st->capacity : STK_MIN_CAP;
	for (int i = 0; i < max; i++) {

		fprintf(output, "%d element: ", i + 1);

		if (StackPrint((void*)((char*)st->data + i * st->itype), output) == 1) {
			printf("StackPrint error\n");
			return 1;
		}
	}

	return 0;
}

int GetTime(char *out) {
	assert(out);
	assert(strlen(out) != 18);

	time_t s_time;
	struct tm* m_time;
	char str_t[18] = "";

	s_time = time(NULL);
	m_time = localtime(&s_time);

	if (strftime(str_t, 18, "%x %X", m_time) == 0) {
		printf("strftime error\n");
		return 1;
	}
	str_t[17] = '\0';
	if (strcpy(out, (const char*)str_t) == NULL) {
		printf("strcpy error\n");
		return 1;
	}

	return 0;
}

int StackDump(Stack* st) {

	int success = 1;
	FILE* log;

	if ((log = fopen("StackLog.txt", "a")) == NULL) {
		printf("FILE OPENING ERROR");
	}

	char* time = (char*)calloc(18, sizeof(char));
	GetTime(time);
	fprintf(log, "%s:\n", time);


	if (fprintf(log, "Stack %s dump:\n", st->name)  == 1) {
		printf("Error: could not print name\n");
		success = 0;
	}

	if (fprintf(log, "Item size in bytes: %d\n", st->itype) == 1) {
		printf("Error: could not print item size \n");
		success = 0;
	}

	if (fprintf(log, "Size: %d\n", st->size) == 1) {
		printf("Error: could not print size \n");
		success = 0;
	}
	if (fprintf(log, "Capacity: %d\n", st->capacity) == 1) {
		printf("Error: could not print capacity \n");
		success = 0;
	}
	if (fprintf(log, "Data %p\n", st->data)  == 1) {
		printf("Error: could not print data pointer\n");
		success = 0;
	}
	if (fprintf(log, "Status: %d", st->status) == 1) {
		printf("Error: could not print status \n");
		success = 0;
	}

	if (st->status == STK_INITIALISED) {
		fprintf(log, " -- initialized\n");
	}
	else if (st->status == STK_NOT_INITIALISED) {
		fprintf(log, " -- not initialized\n");
	}
	else if (st->status == STK_DESTROYED) {
		fprintf(log, " -- destroyed\n");
	}
	else {
		fprintf(log, " -- broken\n");
	}

	if (st->data != NULL) {
		if (AllStackPrint(st, log) == 1) {
			printf("AllStackPrint error\n");
			success = 0;
		}
	}

	fprintf(log, "\n\n");

	fclose(log);

	if (success == 0) {
		return 1;
	}

	else {
		return 0;
	}
}

unsigned long int TrueStackCheck(Stack* st, const char* funcname, const char* filename, int linename) {

	assert(st);

	unsigned long int broken = 0;

	if (st->LCan != LCAN_VALUE) {
		broken |= STK_BAD_LCAN;
	}

	if (st->RCan != RCAN_VALUE) {
		broken |= STK_BAD_RCAN;
	}
	

	if(st->status == STK_NOT_INITIALISED) {
		if (st->size != 0) {
			broken |= STK_BAD_SIZE;
		}
		if (st->capacity != 0) {
			broken |= STK_BAD_CAP;
		}
		if (st->itype != 0) {
			broken |= STK_BAD_ITYPE;
		}
		if (st->data != NULL) {
			broken |= STK_BAD_DATA_PTR;
		}
		if (st->name != NULL) {
			broken |= STK_BAD_NAME;
		}
	}

	else if (st->status == STK_INITIALISED) {
		if (st->size < 0) {
			broken |= STK_BAD_SIZE;
		}
		if (st->capacity < 0) {
			broken |= STK_BAD_CAP;
		}
		if (st->itype <= 0) {
			broken |= STK_BAD_ITYPE;
		}
		if (st->size > st->capacity) {
			broken |= STK_OVERFLOW;
		}
		if (st->data == NULL) {
			broken |= STK_BAD_DATA_PTR;
		}
		if (st->name == NULL) {
			broken |= STK_BAD_NAME;
		}

		if (*st->DataLCanPtr != D_LCAN_VALUE) {
			broken |= STK_BAD_D_LCAN;
		}

		if (*st->DataRCanPtr != D_RCAN_VALUE) {
			broken |= STK_BAD_D_RCAN;
		}

		if (*st->NameLCanPtr != N_LCAN_VALUE) {
			broken |= STK_BAD_N_LCAN;
		}

		if (*st->NameRCanPtr != N_RCAN_VALUE) {
			broken |= STK_BAD_N_RCAN;
		}

#ifdef USEHASH
		unsigned long int temp = st->HashSum;

		if (StackHash(st) != 0 || st->HashSum != temp) {
			printf("StackHash error\n");
			broken |= STK_BAD_HASH;
		}
#endif

	}
	else if (st->status == STK_DESTROYED) {

		if (st->size != -1) {
			broken |= STK_BAD_SIZE;
		}
		if (st->capacity != -1) {
			broken |= STK_BAD_CAP;
		}
		if (st->itype != 0) {
			broken |= STK_BAD_ITYPE;
		}
		if (st->data != (int*)0xBADBAD) {
			broken |= STK_BAD_DATA_PTR;
		}

		if (broken != 0) {
			printf("Destroyed stack %s is broken.\n", st->name);
		}
	}
	else {
		broken |= STK_BAD_STATUS;
	}

	if (broken != 0 && st->status != STK_DESTROYED) {

		printf("Stack %s is broken. Dumping log into StackLog.txt...\n", st->name);

		FILE* log;

		if ((log = fopen("StackLog.txt", "a")) == NULL) {
			printf("Log opening error");
			return broken;
		}
		else {
			fprintf(log, "--------------Called from:\n");
			fprintf(log, "function %s, file %s, line %d\n", funcname, filename, linename);

			fclose(log);

			StackDump(st);
		}
	}
	return broken;
}

int StackPrintError(unsigned long int error) {

	assert(error >= 0);

	int code = 1;
	int printed = 0;

	int totalerrors = 0;

#ifdef USEHASH
	totalerrors = 13;
#else
	totalerrors = 12;
#endif

	for (int i = 0; i <= totalerrors; i++) {

		if (error & code) {
			if (printf(ErrorNames[i]) < 0) {
				printf("Could not print error\n");
				printed = -1;
				break;
			}
			else {
				printed++;
			}
		}
		code *= 2;
	}

	return printed;
}

#ifdef USEHASH
int StackHash(Stack* st) {

	assert(st);

	unsigned long int Sum = 0;

	if (st->status == STK_INITIALISED) {

		Sum += (unsigned long int)(st->size + st->capacity + st->status + st->itype);
		Sum += (unsigned long int)(st->datachunk);
		Sum += (unsigned long int)(st->data);
		Sum += (unsigned long int)(st->DataLCanPtr);
		Sum += (unsigned long int)(st->DataRCanPtr);
		Sum += (unsigned long int)(st->namechunk);
		Sum += (unsigned long int)(st->name);
		Sum += (unsigned long int)(st->NameLCanPtr);
		Sum += (unsigned long int)(st->NameRCanPtr);

		for (int i = 0; i < strlen(st->name); i++) {
			Sum += i * int(*(st->name + i));
		}

		for (int i = 0; i < st->capacity * st->itype; i++) {
			Sum += i * int(*((char *)st->data + i));
		}

		st->HashSum = Sum;

		if (Sum > 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		printf("StackHash error: stack is not initialised\n");
		return 1;
	}



}
#endif


int StackArrangeData(Stack* st) {

	st->data = (char*)st->datachunk + sizeof(canary);
	st->DataLCanPtr = (canary*)st->datachunk;
	st->DataRCanPtr = (canary*)((char*)st->datachunk + st->capacity * st->itype + sizeof(canary));

	if (st->data == NULL || st->DataLCanPtr == NULL || st->DataRCanPtr == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}