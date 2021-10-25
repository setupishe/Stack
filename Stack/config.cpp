#include <stdio.h>
#include "config.h"

int StackPrint(void* ptr, FILE* output) {

	stest one = {};
	one = *(stest*)((char*)ptr);
	if (fprintf(output, "%d %f %lf\n", one.a, one.b, one.c) < 0) {
		printf("Can not print element\n");
		return 1;
	}

	else {
		return 0;
	}
}


