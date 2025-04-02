#include <Windows.h>

#if defined(_DEBUG) || defined(DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Chap4App.h"
#include "Chap6App.h"
#include "Chap7App.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(_DEBUG) || defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// 디버그 모드일 때 메모리 누수 검사(첫줄에 있어야 한다.)
#endif

	try
	{
		//Chap4App theApp(hInstance);	// 객체 생성(싱글톤 생성)
		//Chap6App theApp(hInstance);
		Chap7App theApp(hInstance);
		if (!theApp.Initialize())	// 초기화(윈도우 생성, DirectX 초기화)
			return 0;
		return theApp.Run();		// 메인 루프 진입(메시지 루프 실행, 프레임 처리 및 랜더링)
	}
	catch (DxException& e)
	{
		// 초기화 실패 시 예외 처리 부분
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}