#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "test.h"

/*ToDo:
* 
* ��������� �� ����� ������� ������ � �����
� ��� (� �� ������ �������� � �������)
� ������� (������ ������)
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
    TestPrintError();
    TestCanary();
    //TestStackHash()

    ASSERT(STK_BAD_DATA_PTR); //testing ASSERT macros, not a function because it exits the program
    
    return 0;
}


