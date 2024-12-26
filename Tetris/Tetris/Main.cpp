#include <iostream>
#include "ConsoleDoubleBuffer.h"

#include "Test.h"

#define _TEST_MODE	1	// �׽�Ʈ �ڵ� ����/���ſ� ������

void main()
{
#ifdef _TEST_MODE		// _TEST_MODE�� �����εǾ� ������ �����Ϸ����� ����, �Ǿ� ���� ������ �����Ϸ����� �������� ����
	Test test;
	test.Test_Output();
#endif // _TEST_MODE

	ConsoleDoubleBuffer* pDoubleBuffer = new ConsoleDoubleBuffer();
	pDoubleBuffer->Initialize();

	while (true)
	{
		pDoubleBuffer->Render("Hello World!!!");
	}

	pDoubleBuffer->Destroy();
	delete pDoubleBuffer;
	pDoubleBuffer = nullptr;

	
}