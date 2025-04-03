#include "Chap7_1App.h"

const int gNumFrameResources = 3;

Chap7_1App::Chap7_1App(HINSTANCE hInstance) : D3DApp(hInstance)
{
}

Chap7_1App::~Chap7_1App()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}

bool Chap7_1App::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));	// 커맨드 리스트 리셋

	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildShapeGeometry();
	BuildRenderItems();
	BuildFrameResources();
	BuildDescriptorHeaps();
	BuildConstantBufferViews();	
	BuildPSOs();

	ThrowIfFailed(mCommandList->Close());	// 커맨드 리스트 닫기
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);	// 명령 큐에 명령 리스트 추가

	FlushCommandQueue();					// 커맨드 큐 비우기

	return true;
}

void Chap7_1App::OnResize()
{	
	D3DApp::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Chap7_1App::Update(const GameTimer& gt)
{
	OnKeyboardInput(gt);
	UpdateCamera(gt);

	// 프레임 리소스 중 현재 프레임 리소스를 가져옴
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;	// 0, 1, 2, 0, 1, 2, ...
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	// GPU가 작업을 완료할때까지 대기
	if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	// 상수 버퍼들 업데이트
	UpdateObjectCBs(gt);	// 오브젝트용
	UpdateMainPassCB(gt);	// 메인 패스용
}

void Chap7_1App::Draw(const GameTimer& gt)
{
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;
	ThrowIfFailed(cmdListAlloc->Reset());
	if (mIsWireframe)
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["wireframe"].Get()));
	}
	else
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));
	}
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// 상태 전이
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 렌더 타겟과 깊이 스텐실 버퍼 클리어
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// 렌더 타겟 설정
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	// 디스크립터 힙 설정
	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// 루트 시그니처 설정
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	// 패스 상수 버퍼 설정
	int passCbvIndex = mPassCbvOffset + mCurrFrameResourceIndex;
	auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	passCbvHandle.Offset(passCbvIndex, mCbvSrvUavDescriptorSize);
	mCommandList->SetGraphicsRootDescriptorTable(1, passCbvHandle);

	// 렌더 아이템 그리기
	DrawRenderItems(mCommandList.Get(), mOpaqueRitems);

	// 상태 전이
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 커맨드 리스트 닫기
	ThrowIfFailed(mCommandList->Close());

	// 명령
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// 스왑체인 진행
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// 프레임 리소스에 펜스 값 설정
	mCurrFrameResource->Fence = ++mCurrentFence;

	// 명령 큐에 펜스 값 설정
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
}

void Chap7_1App::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
}

void Chap7_1App::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Chap7_1App::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// 마우스 왼쪽 버튼이 눌려졌다.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		mTheta += dx;
		mPhi += dy;

		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// 마우스 오른쪽 버튼이 눌려졌다.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		mRadius += dx - dy;

		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Chap7_1App::OnKeyboardInput(const GameTimer& gt)
{
	if (GetAsyncKeyState('1') & 0x8000) //1번키가 눌려졌는지 확인(true면 눌러짐, false 안눌려짐)
	{
		mIsWireframe = true;
	}
	else
	{
		mIsWireframe = false;
	}		
}

void Chap7_1App::UpdateCamera(const GameTimer& gt)
{
	// 구 좌표계를 이용하여 카메라 위치 계산
	mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
	mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
	mEyePos.y = mRadius * cosf(mPhi);

	// 뷰 행렬 계산
	XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);	// 왼손좌표계에서 pos위치에서 target을 바라보는 행렬 생성
	XMStoreFloat4x4(&mView, view);
}

void Chap7_1App::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for (auto& e : mAllRitems)
	{
		if (e->NumFramesDirty > 0)	// NumFramesDirty가 0보다 크면(변	화가 있으면) 업데이트
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);	
			ObjectConstants objConstants;	
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));	
			currObjectCB->CopyData(e->ObjCBIndex, objConstants);	// 상수 버퍼에 월드 행렬 전달
			e->NumFramesDirty--;	// NumFramesDirty 감소
		}
	}
}

void Chap7_1App::UpdateMainPassCB(const GameTimer& gt)
{
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);	// 뷰 행렬과 투영 행렬을 곱함
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);	// 역행렬 계산
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);	// 역행렬 계산
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);	// 역행렬 계산

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));	// 뷰 행렬 전달
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));	// 역행렬 전달
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));	// 투영 행렬 전달
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));	// 역행렬 전달
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));	// 뷰 행렬과 투영 행렬 곱한 행렬 전달
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));	// 역행렬 전달

	mMainPassCB.EyePosW = mEyePos;	// 카메라 위치 전달
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);	// 렌더 타겟 크기 전달
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);	// 역 렌더 타겟 크기 전달
	mMainPassCB.NearZ = 1.0f;	// NearZ 전달
	mMainPassCB.FarZ = 1000.0f;	// FarZ 전달
	mMainPassCB.TotalTime = gt.TotalTime();	// 총 시간 전달
	mMainPassCB.DeltaTime = gt.DeltaTime();	// 델타 시간 전달

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);	// 메인 패스 상수 버퍼에 전달
}

void Chap7_1App::BuildDescriptorHeaps()
{
	UINT objCount = (UINT)mOpaqueRitems.size();

	// 디스크립터의 개수 = (오브젝트 개수 + 1) * 프레임 리소스 개수
	// +1은 PassConstants(mMainPassCB)를 위한 것
	UINT numDescriptors = (objCount + 1) * gNumFrameResources;

	mPassCbvOffset = objCount * gNumFrameResources;	// offset 설정(mMainPassCB 시작위치)

	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = numDescriptors;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
}

void Chap7_1App::BuildConstantBufferViews()
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT objCount = (UINT)mOpaqueRitems.size();

	for (int frameIndex = 0; frameIndex < gNumFrameResources; ++frameIndex)
	{
		auto objectCB = mFrameResources[frameIndex]->ObjectCB->Resource();
		
		// 각 프레임별로 상수 버퍼 뷰 생성
		for (UINT i = 0; i < objCount; ++i)
		{
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = objectCB->GetGPUVirtualAddress();	// 시작 주소(상수 버퍼 전체의 주소)
			cbAddress += i * objCBByteSize;	// i번째 상수 버퍼 주소

			int heapIndex = frameIndex * objCount + i;	// 디스크립터 힙 인덱스
			auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart()); // 디스크립터 힙 시작 주소(CPU)
			handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);	// heapIndex만큼 offset
			
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;		// 상수 버퍼 주소
			cbvDesc.SizeInBytes = objCBByteSize;	// 상수 버퍼 크기
			md3dDevice->CreateConstantBufferView(&cbvDesc, handle);	// 상수 버퍼 뷰 생성
		}
	}

	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));
	for (int frameIndex = 0; frameIndex < gNumFrameResources; ++frameIndex)
	{
		auto passCB = mFrameResources[frameIndex]->PassCB->Resource();
		
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = passCB->GetGPUVirtualAddress();	// 시작 주소(상수 버퍼 전체의 주소)
		
		int heapIndex = mPassCbvOffset + frameIndex;	// 디스크립터 힙 인덱스
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart()); // 디스크립터 힙 시작 주소(CPU)
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);	// heapIndex만큼 offset
		
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;		// 상수 버퍼 주소
		cbvDesc.SizeInBytes = passCBByteSize;	// 상수 버퍼 크기
		md3dDevice->CreateConstantBufferView(&cbvDesc, handle);	// 상수 버퍼 뷰 생성
	}
}

void Chap7_1App::BuildRootSignature()
{
	// 상수버퍼가 2개라 테이블도 2개 생성
	CD3DX12_DESCRIPTOR_RANGE cbvTable0;
	cbvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	CD3DX12_DESCRIPTOR_RANGE cbvTable1;
	cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	CD3DX12_ROOT_PARAMETER slotRootParamater[2];
	slotRootParamater[0].InitAsDescriptorTable(1, &cbvTable0);
	slotRootParamater[1].InitAsDescriptorTable(1, &cbvTable1);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
		2, slotRootParamater, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	ThrowIfFailed(hr);
	ThrowIfFailed(md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(), IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}

void Chap7_1App::BuildShadersAndInputLayout()
{
	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\color_App7.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\color_App7.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void Chap7_1App::BuildShapeGeometry()
{
	// 기본 메시 생성
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(20.0f, 30.0f, 60, 40);
	GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);
	GeometryGenerator::MeshData geoSphere = geoGen.CreateGeosphere(0.5f, 2);

	// 버텍스 버퍼에서 사용할 각 메시의 시작 오프셋을 저장
	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();
	UINT geoSphereVertexOffset = cylinderVertexOffset + (UINT)cylinder.Vertices.size();

	// 각 인덱스 버퍼에서 사용할 각 메시의 시작 오프셋을 저장
	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();
	UINT geoSphereIndexOffset = cylinderIndexOffset + (UINT)cylinder.Indices32.size();

	// 서브메시 지오메트리 기록
	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	SubmeshGeometry geoSphereSubmesh;
	geoSphereSubmesh.IndexCount = (UINT)geoSphere.Indices32.size();
	geoSphereSubmesh.StartIndexLocation = geoSphereIndexOffset;
	geoSphereSubmesh.BaseVertexLocation = geoSphereVertexOffset;

	// 정점 버퍼 전체 크기 계산
	auto totalVertexCount = 
		box.Vertices.size() 
		+ grid.Vertices.size() 
		+ sphere.Vertices.size() 
		+ cylinder.Vertices.size() 
		+ geoSphere.Vertices.size();
	
	// 정점 버퍼 생성하고 채우기
	std::vector<Vertex> vertices(totalVertexCount);
	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::DarkGreen);
	}
	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::ForestGreen);
	}
	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::Crimson);
	}
	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::SteelBlue);
	}
	for (size_t i = 0; i < geoSphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = geoSphere.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::Yellow);
	}

	// 인덱스 버퍼 생성하고 채우기
	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));
	indices.insert(indices.end(), std::begin(geoSphere.GetIndices16()), std::end(geoSphere.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";

	// 정점 버퍼와 인덱스 버퍼 생성(CPU와 GPU)
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);		// 정점 하나의 크기 저장(GPU가 읽을 때 사용)
	geo->VertexBufferByteSize = vbByteSize;		// 정점 버퍼 크기 저장
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;	// 인덱스 형식 저장
	geo->IndexBufferByteSize = ibByteSize;		// 인덱스 버퍼 크기 저장

	geo->DrawArgs["box"] = boxSubmesh;			// 맵에 서브메시 설정
	geo->DrawArgs["grid"] = gridSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["cylinder"] = cylinderSubmesh;
	geo->DrawArgs["geoSphere"] = geoSphereSubmesh;

	mGeometries[geo->Name] = std::move(geo);	// 맵에 지오메트리 추가
}

void Chap7_1App::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	// 불투명한 오브젝트 용
	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));

	// 와이어 프레임 용
	D3D12_GRAPHICS_PIPELINE_STATE_DESC wireframePsoDesc = opaquePsoDesc;
	wireframePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&wireframePsoDesc, IID_PPV_ARGS(&mPSOs["wireframe"])));
}

void Chap7_1App::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(
			std::make_unique<FrameResource>(md3dDevice.Get(), 1, (UINT)mAllRitems.size()));
	}
}

void Chap7_1App::BuildRenderItems()
{
	auto boxRitem = std::make_unique<RenderItemApp7>();
	XMStoreFloat4x4(&boxRitem->World, XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, 0.5f, 0.0f));
	boxRitem->ObjCBIndex = 0;
	boxRitem->Geo = mGeometries["shapeGeo"].get();
	boxRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	boxRitem->IndexCount = boxRitem->Geo->DrawArgs["box"].IndexCount;
	boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs["box"].StartIndexLocation;
	boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs["box"].BaseVertexLocation;
	mAllRitems.push_back(std::move(boxRitem));

	auto gridRitem = std::make_unique<RenderItemApp7>();
	gridRitem->World = MathHelper::Identity4x4();
	gridRitem->ObjCBIndex = 1;
	gridRitem->Geo = mGeometries["shapeGeo"].get();
	gridRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	gridRitem->IndexCount = gridRitem->Geo->DrawArgs["grid"].IndexCount;
	gridRitem->StartIndexLocation = gridRitem->Geo->DrawArgs["grid"].StartIndexLocation;
	gridRitem->BaseVertexLocation = gridRitem->Geo->DrawArgs["grid"].BaseVertexLocation;
	mAllRitems.push_back(std::move(gridRitem));

	UINT objCBIndex = 2;
	for (int i = 0; i < 5; ++i)
	{
		auto leftCylRitem = std::make_unique<RenderItemApp7>();
		auto rightCylRitem = std::make_unique<RenderItemApp7>();
		auto leftSphereRitem = std::make_unique<RenderItemApp7>();
		auto rightSphereRitem = std::make_unique<RenderItemApp7>();

		XMMATRIX leftCylWorld = XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f);
		XMMATRIX rightCylWorld = XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i * 5.0f);
		XMMATRIX leftSphereWorld = XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f);
		XMMATRIX rightSphereWorld = XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i * 5.0f);

		XMStoreFloat4x4(&leftCylRitem->World, leftCylWorld);
		leftCylRitem->ObjCBIndex = objCBIndex++;
		leftCylRitem->Geo = mGeometries["shapeGeo"].get();
		leftCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		leftCylRitem->IndexCount = leftCylRitem->Geo->DrawArgs["cylinder"].IndexCount;
		leftCylRitem->StartIndexLocation = leftCylRitem->Geo->DrawArgs["cylinder"].StartIndexLocation;
		leftCylRitem->BaseVertexLocation = leftCylRitem->Geo->DrawArgs["cylinder"].BaseVertexLocation;

		XMStoreFloat4x4(&rightCylRitem->World, rightCylWorld);
		rightCylRitem->ObjCBIndex = objCBIndex++;
		rightCylRitem->Geo = mGeometries["shapeGeo"].get();
		rightCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		rightCylRitem->IndexCount = rightCylRitem->Geo->DrawArgs["cylinder"].IndexCount;
		rightCylRitem->StartIndexLocation = rightCylRitem->Geo->DrawArgs["cylinder"].StartIndexLocation;
		rightCylRitem->BaseVertexLocation = rightCylRitem->Geo->DrawArgs["cylinder"].BaseVertexLocation;

		XMStoreFloat4x4(&leftSphereRitem->World, leftSphereWorld);
		leftSphereRitem->ObjCBIndex = objCBIndex++;
		leftSphereRitem->Geo = mGeometries["shapeGeo"].get();
		leftSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		leftSphereRitem->IndexCount = leftSphereRitem->Geo->DrawArgs["sphere"].IndexCount;
		leftSphereRitem->StartIndexLocation = leftSphereRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
		leftSphereRitem->BaseVertexLocation = leftSphereRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;

		XMStoreFloat4x4(&rightSphereRitem->World, rightSphereWorld);
		rightSphereRitem->ObjCBIndex = objCBIndex++;
		rightSphereRitem->Geo = mGeometries["shapeGeo"].get();
		rightSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		rightSphereRitem->IndexCount = rightSphereRitem->Geo->DrawArgs["sphere"].IndexCount;
		rightSphereRitem->StartIndexLocation = rightSphereRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
		rightSphereRitem->BaseVertexLocation = rightSphereRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;

		mAllRitems.push_back(std::move(leftCylRitem));
		mAllRitems.push_back(std::move(rightCylRitem));
		mAllRitems.push_back(std::move(leftSphereRitem));
		mAllRitems.push_back(std::move(rightSphereRitem));
	}

	auto geoSphereRitem = std::make_unique<RenderItemApp7>();
	XMStoreFloat4x4(&geoSphereRitem->World, XMMatrixTranslation(0.0f, 2.0f, 0.0f));
	geoSphereRitem->ObjCBIndex = objCBIndex;
	geoSphereRitem->Geo = mGeometries["shapeGeo"].get();
	geoSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	geoSphereRitem->IndexCount = geoSphereRitem->Geo->DrawArgs["geoSphere"].IndexCount;
	geoSphereRitem->StartIndexLocation = geoSphereRitem->Geo->DrawArgs["geoSphere"].StartIndexLocation;
	geoSphereRitem->BaseVertexLocation = geoSphereRitem->Geo->DrawArgs["geoSphere"].BaseVertexLocation;
	mAllRitems.push_back(std::move(geoSphereRitem));

	for (auto& e : mAllRitems)
		mOpaqueRitems.push_back(e.get());
}

void Chap7_1App::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp7*>& ritems)
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	auto objectCB = mCurrFrameResource->ObjectCB->Resource();

	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		// IA 설정
		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());	// 정점 버퍼 설정
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());				// 인덱스 버퍼 설정
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);					// 프리미티브 타입 설정

		UINT cbvIndex = mCurrFrameResourceIndex * (UINT)mOpaqueRitems.size() + ri->ObjCBIndex;	// 상수 버퍼 인덱스 계산
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		cbvHandle.Offset(cbvIndex, mCbvSrvUavDescriptorSize);	// 상수 버퍼 디스크립터 힙 시작 주소에서 offset

		cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);	// 루트 디스크립터 테이블 설정
		cmdList->DrawIndexedInstanced(
			ri->IndexCount, 
			1, 
			ri->StartIndexLocation, 
			ri->BaseVertexLocation, 0);	// 드로우 호출
	}
}