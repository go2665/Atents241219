#include <iostream>
#include <Windows.h>
#include "Test.h"

void Test::Test_Output()
{
	printf("�ѱ�");
	OutputDebugString(L"Print~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	OutputDebugStringA("Print-------------------------\n");
}
