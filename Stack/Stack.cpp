#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "test.h"

/*ToDo:

*/

int main()
{   
    TestStackNamer("&dummy");
    TestStackCtor();
    TestPushPop(10);
    TestStackDtor();
    TestStackResize(STK_MIN_CAP * 200);
    TestStackDump();
    TestStackCheck();
    TestCanary();
    
    return 0;
}


