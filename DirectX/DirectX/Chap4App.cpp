#include "Chap4App.h"
#include <DirectXColors.h>

using namespace DirectX;

Chap4App::Chap4App(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
	mMainWndCaption = L"Chapter4 App";
}

Chap4App::~Chap4App()
{
}

void Chap4App::Draw(const GameTimer& gt)
{
    // 커맨드 리스트 할당자를 재사용하기 위해 리셋.
	// 재설정은 GPU가 커맨드 리스트에 있는 모든 명령을 완료했을 때만 재설정할 수 있습니다.
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    // 커맨드 리스트는 ExecuteCommandList를 통해 명령 큐에 추가된 후 재설정할 수 있습니다.        
    // 커맨드 리스트를 재사용하면 메모리를 재사용할 수 있습니다.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    // 리소스 사용 상태 전환을 나타냅니다.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // 뷰포트와 가위 사각형을 설정합니다. 커맨드리스트가 재설정될 때마다 이를 재설정해야 합니다.
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // 백 버퍼와 깊이 버퍼를 클리어한다.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // 렌더링할 버퍼(렌터타겟)를 지정합니다.
    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    // 리소스 사용에 대한 상태 전환을 나타냅니다.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // 커맨드리스트 기록 완료.
    ThrowIfFailed(mCommandList->Close());

    // 실행을 위해 명령 목록을 큐에 추가합니다.(GPU에게 명령을 전달)
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // 백 버퍼와 프론트 버퍼를 교체합니다.
    ThrowIfFailed(mSwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // 프레임 명령이 완료될 때까지 기다립니다. 이 대기는 비효율적이며 단순화를 위해 수행됩니다. 나중에 프레임마다 기다릴 필요가 없도록 렌더링 코드를 구성하는 방법을 보여드리겠습니다.
    FlushCommandQueue();
}
