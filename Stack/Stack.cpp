#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "test.h"

/*ToDo:
To file
Macros
StackCheck->Macros
*/

int main()
{   

    TestStackNamer("&dummy");
    TestStackCtor();
    TestPushPop(10);
    TestStackDtor();
    TestStackResize(MIN_CAP * 200);
    TestStackDump();
    TestStackCheck();
    
    return 0;
}


