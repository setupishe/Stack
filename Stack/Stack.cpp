

#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "test.h"

int main()
{
    TestStackCtor();
    TestPushPop(10);
    TestStackDtor();
    TestStackResize(MIN_CAP * 200);

    return 0;
}


