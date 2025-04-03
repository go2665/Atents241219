//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"    // D3D관련 유틸리티 함수 및 클래스들
#include "GameTimer.h"  // 시간 측정용 클래스

// Link necessary d3d12 libraries.
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3DApp
{
protected:
	// 싱글톤 처리를 위해 생성자 및 대입 연산자 삭제
    D3DApp(HINSTANCE hInstance);
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp();

public:
	// 싱글톤 객체를 반환하는 정적 함수
    static D3DApp* GetApp();
    
	HINSTANCE AppInst()const;		// 프로그램 인스턴스 핸들을 반환하는 함수
	HWND      MainWnd()const;		// 메인 윈도우 핸들을 반환하는 함수
	float     AspectRatio()const;	// 화면의 가로 세로 비율을 반환하는 함수

	bool Get4xMsaaState()const;			// 4X MSAA 사용 여부를 반환하는 함수
	void Set4xMsaaState(bool value);	// 4X MSAA 사용 여부를 설정하는 함수

	int Run();	// 메인 루프 함수(메시지 루프 처리 및 프레임 처리 및 랜더링)
 
	virtual bool Initialize();		// 윈도우 생성 및 DirectX 초기화 함수
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	// 메시지 프로시저 함수

protected:
    virtual void CreateRtvAndDsvDescriptorHeaps();	// 디스크립터 힙 생성 함수(리소스 관리용)
	virtual void OnResize();						// 윈도우 크기가 변경되었을 때 호출되는 함수
	virtual void Update(const GameTimer& gt) = 0;	// 게임 로직 업데이트 용 가상함수
	virtual void Draw(const GameTimer& gt) = 0;		// 랜더링 용 가상함수

	// Convenience overrides for handling mouse input. 마우스 입력처리용 편의성함수
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

protected:

	bool InitMainWindow();		// 메인 윈도우 생성 함수
	bool InitDirect3D();		// DirectX 초기화 함수
	void CreateCommandObjects();	// 커맨드 객체 생성 함수
	void CreateSwapChain();			// 스왑체인 생성 함수(더블 버퍼링용)

	void FlushCommandQueue();	// 커맨드 큐를 전부 처리하라는 함수

	ID3D12Resource* CurrentBackBuffer()const;	// 현재 백버퍼를 반환하는 함수(직접 접근)
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;	// 현재 백버퍼의 디스크립터 핸들을 반환하는 함수
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;		// 깊이 스텐실 뷰의 디스크립터 핸들을 반환하는 함수

	void CalculateFrameStats();	// 프레임 통계를 계산하는 함수(프레임 시간, FPS 계산)

	// 로그 함수들
	void LogAdapters();	// 어댑터(그래픽카드) 정보 로그
	void LogAdapterOutputs(IDXGIAdapter* adapter);	// 어댑터의 출력 장치 로그
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);	// 출력 장치의 디스플레이 모드 로그(해상도, 주사율)

protected:

    static D3DApp* mApp;	// 싱글톤 객체

    HINSTANCE mhAppInst = nullptr; // application instance handle(OS가 프로그램을 식별하고 접근하기 위해 사용)
    HWND      mhMainWnd = nullptr; // main window handle(OS에서 프로그램 창을 식별하고 접근하기 위해 사용)
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (�4.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the �delta-time� and game time (�4.4). 타이머용 객체
	GameTimer mTimer;
	
	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;	// DXGI 팩토리 객체(그래픽 디바이스와 스왑체인을 생성하고 관리)
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;	// 스왑체인 객체(더블 버퍼링을 위한 스왑체인)
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;	// D3D12 디바이스 객체(그래픽 리소스 생성 및 관리)

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;	// 펜스 객체(CPU와 GPU를 동기화하기 위한 객체, GPU가 특정 작업을 완료했는지 확인하는데 사용))
    UINT64 mCurrentFence = 0;
	
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;	// 커맨드 큐 객체(그래픽 명령을 GPU에 전달하는 큐)
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc; // 커맨드 할당자 객체(그래픽 명령을 기록하기 위한 메모리 할당자)
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList; // 커맨드 리스트 객체(그래픽 명령을 기록하는데 사용)

	static const int SwapChainBufferCount = 2;	// 더블버퍼링 사용(2개니까)
	int mCurrBackBuffer = 0;					// 현재 백버퍼 인덱스
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];	// 스왑체인 버퍼(더블버퍼링용)
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;	// 깊이 스텐실 버퍼

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;	// 렌더 타겟 뷰 디스크립터 힙(렌더 타겟 뷰를 관리하는데 사용)
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;	// 깊이 스텐실 뷰 디스크립터 힙(깊이 스텐실 뷰를 관리하는데 사용)

	D3D12_VIEWPORT mScreenViewport; // 화면 뷰포트(화면에 랜더링되는 영역)
	D3D12_RECT mScissorRect;		// 클리핑 영역

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// 파생 클래스에서 이것들을 설정하여 시작 값을 사용자 정의해야 함
	std::wstring mMainWndCaption = L"d3d App";	// 윈도우 캡션
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;		// 클라이언트 가로 크기
	int mClientHeight = 600;	// 클라이언트 세로 크기
};

