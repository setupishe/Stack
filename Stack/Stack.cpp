#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "test.h"

/*ToDo:
add every case to stackcheck test func

*/

int main()
{   
    TestStackNamer("&dummy");
    TestStackCtor();
    TestPushPop(10);
    TestStackDtor();
    TestStackResize(STACK_MIN_CAP * 200);
    TestStackDump();
    TestStackCheck();
    
    return 0;
}


