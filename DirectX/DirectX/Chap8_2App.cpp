#include "Chap8_2App.h"

Chap8_2App::Chap8_2App(HINSTANCE hInstance) : D3DApp(hInstance)
{
}

Chap8_2App::~Chap8_2App()
{
	if (md3dDevice != nullptr)
	{
		FlushCommandQueue();	// 커맨드 큐를 전부 처리하라는 함수
	}
}

bool Chap8_2App::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));	// 커맨드 리스트 리셋
	mWaves = std::make_unique<Waves>(128, 128, 1.0f, 0.03f, 4.0f, 0.2f);	// 파도 객체 생성

	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildLandGeometry();
	BuildWavesGeometryBuffers();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSOs();

	ThrowIfFailed(mCommandList->Close());	// 커맨드 리스트 닫기
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);	// 명령 큐에 명령 리스트 추가

	FlushCommandQueue();					// 커맨드 큐 비우기

	return true;
}

void Chap8_2App::OnResize()
{
	D3DApp::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Chap8_2App::Update(const GameTimer& gt)
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
	UpdateMaterialCBs(gt);	// 머티리얼용
	UpdateMainPassCB(gt);	// 메인 패스용

	// 파도 업데이트
	UpdateWaves(gt);
}

void Chap8_2App::Draw(const GameTimer& gt)
{
	// 커맨드 리스트 할당자 리셋
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;
	ThrowIfFailed(cmdListAlloc->Reset());

	// 커맨드 리스트 리셋(와이어프레임인지 아닌지에 따라 따로 처리)
	if (mIsWireframe)
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["wireframe"].Get()));
	}
	else
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));
	}

	// 뷰포트와 가위렉트 설정
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

	// 루트 시그니처 설정
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	// 패스별로 상수버퍼 바인딩
	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());	// 패스 상수 버퍼 설정

	// 렌더 아이템 그리기
	DrawRenderItems(mCommandList.Get(), mRitemLayer[static_cast<int>(RenderLayer::Opaque)]);

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

void Chap8_2App::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
}

void Chap8_2App::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Chap8_2App::OnMouseMove(WPARAM btnState, int x, int y)
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
		float dx = 0.2f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.2f * static_cast<float>(y - mLastMousePos.y);

		mRadius += dx - dy;

		mRadius = MathHelper::Clamp(mRadius, 5.0f, 150.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Chap8_2App::OnKeyboardInput(const GameTimer& gt)
{
	const float dt = gt.DeltaTime();

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) // 왼쪽 화살표키가 눌려졌는지 확인(true면 눌러짐, false 안눌려짐)
	{
		mSunTheta -= 1.0f * dt;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // 오른쪽 화살표키가 눌려졌는지 확인(true면 눌러짐, false 안눌려짐)
	{
		mSunTheta += 1.0f * dt;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000) // 위쪽 화살표키가 눌려졌는지 확인(true면 눌러짐, false 안눌려짐)
	{
		mSunPhi -= 1.0f * dt;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) // 아래쪽 화살표키가 눌려졌는지 확인(true면 눌러짐, false 안눌려짐)
	{
		mSunPhi += 1.0f * dt;
	}

	mSunPhi = MathHelper::Clamp(mSunPhi, 0.1f, XM_PIDIV2);	// 해가 위쪽에 있도록 제한


	if (GetAsyncKeyState('1') & 0x8000) //1번키가 눌려졌는지 확인(true면 눌러짐, false 안눌려짐)
	{
		mIsWireframe = true;
	}
	else
	{
		mIsWireframe = false;
	}
}

void Chap8_2App::UpdateCamera(const GameTimer& gt)
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

void Chap8_2App::UpdateObjectCBs(const GameTimer& gt)
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

void Chap8_2App::UpdateMaterialCBs(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	for (auto& e : mMaterials)
	{
		Material* mat = e.second.get();
		if (mat->NumFramesDirty > 0)	// NumFramesDirty가 0보다 크면(변화가 있으면) 업데이트
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialConstants matConstants;
			matConstants.DiffuseAlbedo = mat->DiffuseAlbedo;
			matConstants.FresnelR0 = mat->FresnelR0;
			matConstants.Roughness = mat->Roughness;
			//XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));
			currMaterialCB->CopyData(mat->MatCBIndex, matConstants);	// 상수 버퍼에 월드 행렬 전달
			e.second->NumFramesDirty--;	// NumFramesDirty 감소
		}
	}
}

void Chap8_2App::UpdateMainPassCB(const GameTimer& gt)
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

	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	XMVECTOR lightDir = -MathHelper::SphericalToCartesian(1.0f, mSunTheta, mSunPhi);
	XMStoreFloat3(&mMainPassCB.Lights[0].Direction, lightDir);	// 조명 방향 전달
	mMainPassCB.Lights[0].Strength = { 1.0f, 1.f, 0.9f};

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);	// 메인 패스 상수 버퍼에 전달
}

void Chap8_2App::UpdateWaves(const GameTimer& gt)
{
	static float t_bast = 0.0f;

	// 0.25초마다 새 파도 생성
	if ((mTimer.TotalTime() - t_bast) >= 0.25f)
	{
		t_bast += 0.25f;

		int i = MathHelper::Rand(4, mWaves->RowCount() - 5);
		int j = MathHelper::Rand(4, mWaves->ColumnCount() - 5);

		float r = MathHelper::RandF(0.2f, 0.5f);

		mWaves->Disturb(i, j, r);
	}

	// 파도 업데이트
	mWaves->Update(gt.DeltaTime());

	// 파도 정점 버퍼 업데이트
	auto currWavesVB = mCurrFrameResource->WavesVB.get();
	for (int i = 0; i < mWaves->VertexCount(); i++)
	{
		VertexLight v;
		v.Pos = mWaves->Position(i);	// 파도 높이 계산
		v.Normal = mWaves->Normal(i);	// 파도 법선 계산
		currWavesVB->CopyData(i, v);	// 정점 버퍼에 복사
	}
	mWavesRitem->Geo->VertexBufferGPU = currWavesVB->Resource();	// 정점 버퍼 GPU에 복사
}

void Chap8_2App::BuildRootSignature()
{
	CD3DX12_ROOT_PARAMETER slotRootParamater[3];
	slotRootParamater[0].InitAsConstantBufferView(0);
	slotRootParamater[1].InitAsConstantBufferView(1);
	slotRootParamater[2].InitAsConstantBufferView(2);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
		3, slotRootParamater, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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

void Chap8_2App::BuildShadersAndInputLayout()
{
	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void Chap8_2App::BuildLandGeometry()
{
	// 기본 메시 생성
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(160.0f, 160.0f, 50, 50);

	// 정점 버퍼 생성하고 채우기
	std::vector<VertexLight> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		auto& p = grid.Vertices[i].Position;
		vertices[i].Pos = p;
		vertices[i].Pos.y = GetHillsHeight(p.x, p.z);	// 언덕 높이 계산
		vertices[i].Normal = GetHillsNormal(p.x, p.z);	// 언덕 법선 계산		
	}

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(VertexLight);

	std::vector<std::uint16_t> indices(grid.GetIndices16());
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "landGeo";

	// 정점 버퍼와 인덱스 버퍼 생성(CPU와 GPU)
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);
	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(VertexLight);		// 정점 하나의 크기 저장(GPU가 읽을 때 사용)
	geo->VertexBufferByteSize = vbByteSize;		// 정점 버퍼 크기 저장
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;	// 인덱스 포맷
	geo->IndexBufferByteSize = ibByteSize;		// 인덱스 버퍼 크기 저장

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	geo->DrawArgs["grid"] = submesh;			// 서브메시 추가

	mGeometries["landGeo"] = std::move(geo);	// 맵에 지오메트리 추가
}

void Chap8_2App::BuildWavesGeometryBuffers()
{
	std::vector<std::uint16_t> indices(3 * mWaves->TriangleCount());	// 인덱스 개수는 삼각형 개수 * 3
	assert(mWaves->VertexCount() < 0x0000FFFF);	// 인덱스 개수는 16비트로 표현 가능해야 함

	// 모든 그리드 사각형을 순회
	int m = mWaves->RowCount();
	int n = mWaves->ColumnCount();
	int k = 0;
	for (int i = 0; i < m - 1; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			k += 6;	// 사각형 하나 완료
		}
	}
	const UINT vbByteSize = mWaves->VertexCount() * sizeof(VertexLight);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "waveGeo";

	geo->VertexBufferCPU = nullptr;	// Update에서 매번 새로 생성
	geo->VertexBufferGPU = nullptr;

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);
	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(VertexLight);		// 정점 하나의 크기 저장(GPU가 읽을 때 사용)
	geo->VertexBufferByteSize = vbByteSize;		// 정점 버퍼 크기 저장
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;	// 인덱스 포맷
	geo->IndexBufferByteSize = ibByteSize;		// 인덱스 버퍼 크기 저장

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	geo->DrawArgs["grid"] = submesh;			// 서브메시 추가
	mGeometries["waveGeo"] = std::move(geo);	// 맵에 지오메트리 추가
}

void Chap8_2App::BuildPSOs()
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

void Chap8_2App::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(
			std::make_unique<FrameResourceMaterialWaves>(
				md3dDevice.Get(), 1, (UINT)mAllRitems.size(), mWaves->VertexCount(), mWaves->VertexCount())
		);
	}
}

void Chap8_2App::BuildMaterials()
{
	auto grass = std::make_unique<Material>();
	grass->Name = "grass";
	grass->MatCBIndex = 0;
	grass->DiffuseAlbedo = XMFLOAT4(0.2f, 0.6f, 0.2f, 1.0f);
	grass->FresnelR0 = XMFLOAT3(0.01f, 0.01f, 0.01f);
	grass->Roughness = 0.125f;

	auto water = std::make_unique<Material>();
	water->Name = "water";
	water->MatCBIndex = 1;
	water->DiffuseAlbedo = XMFLOAT4(0.0f, 0.2f, 0.6f, 1.0f);
	water->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	water->Roughness = 0.0f;

	mMaterials["grass"] = std::move(grass);
	mMaterials["water"] = std::move(water);
}

void Chap8_2App::BuildRenderItems()
{
	auto wavesRitem = std::make_unique<RenderItemApp8>();
	wavesRitem->World = MathHelper::Identity4x4();
	wavesRitem->ObjCBIndex = 0;
	wavesRitem->Mat = mMaterials["water"].get();	// 머티리얼 설정
	wavesRitem->Geo = mGeometries["waveGeo"].get();
	wavesRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	wavesRitem->IndexCount = wavesRitem->Geo->DrawArgs["grid"].IndexCount;
	wavesRitem->StartIndexLocation = wavesRitem->Geo->DrawArgs["grid"].StartIndexLocation;
	wavesRitem->BaseVertexLocation = wavesRitem->Geo->DrawArgs["grid"].BaseVertexLocation;

	mWavesRitem = wavesRitem.get();

	mRitemLayer[static_cast<int>(RenderLayer::Opaque)].push_back(wavesRitem.get());	// 렌더 레이어에 추가

	auto gridRitem = std::make_unique<RenderItemApp8>();
	gridRitem->World = MathHelper::Identity4x4();
	gridRitem->ObjCBIndex = 1;
	gridRitem->Mat = mMaterials["grass"].get();	// 머티리얼 설정
	gridRitem->Geo = mGeometries["landGeo"].get();
	gridRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	gridRitem->IndexCount = gridRitem->Geo->DrawArgs["grid"].IndexCount;
	gridRitem->StartIndexLocation = gridRitem->Geo->DrawArgs["grid"].StartIndexLocation;
	gridRitem->BaseVertexLocation = gridRitem->Geo->DrawArgs["grid"].BaseVertexLocation;

	mRitemLayer[static_cast<int>(RenderLayer::Opaque)].push_back(gridRitem.get());	// 렌더 레이어에 추가

	mAllRitems.push_back(std::move(wavesRitem));	// 렌더 아이템 추가
	mAllRitems.push_back(std::move(gridRitem));
}

void Chap8_2App::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp8*>& ritems)
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstantsTex));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
	auto materialCB = mCurrFrameResource->MaterialCB->Resource();

	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		// IA 설정
		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());	// 정점 버퍼 설정
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());				// 인덱스 버퍼 설정
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);					// 프리미티브 타입 설정

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress();
		objCBAddress += ri->ObjCBIndex * objCBByteSize;						// 오브젝트 상수 버퍼 주소 계산

		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = materialCB->GetGPUVirtualAddress();
		matCBAddress += ri->Mat->MatCBIndex * matCBByteSize;				// 머티리얼 상수 버퍼 주소 계산

		cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);		// 오브젝트 상수 버퍼 설정
		cmdList->SetGraphicsRootConstantBufferView(1, matCBAddress);		// 머티리얼 상수 버퍼 설정

		cmdList->DrawIndexedInstanced(
			ri->IndexCount,
			1,
			ri->StartIndexLocation,
			ri->BaseVertexLocation,
			0);	// 드로우 호출
	}
}

float Chap8_2App::GetHillsHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

XMFLOAT3 Chap8_2App::GetHillsNormal(float x, float z) const
{
	XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z)
	);
	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));	// 정규화
	XMStoreFloat3(&n, unitNormal);	// 정규화된 벡터를 n에 저장
	return n;
}
