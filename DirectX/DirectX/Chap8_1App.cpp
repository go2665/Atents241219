#include "Chap8_1App.h"

Chap8_1App::Chap8_1App(HINSTANCE hInstance) : D3DApp(hInstance)
{
}

Chap8_1App::~Chap8_1App()
{
	if (md3dDevice != nullptr)
	{
		FlushCommandQueue();	// 커맨드 큐를 전부 처리하라는 함수
	}
}

bool Chap8_1App::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));	// 커맨드 리스트 리셋

	mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);	// CBV/SRV/UAV 디스크립터 핸들 크기

	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildShapeGeometry();
	BuildSkullGeometry();
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

void Chap8_1App::OnResize()
{
	D3DApp::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);	// 프로젝션 행렬 저장
}

void Chap8_1App::Update(const GameTimer& gt)
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

	AnimateMaterials(gt);	// 머티리얼 애니메이션 업데이트

	// 상수 버퍼들 업데이트
	UpdateObjectCBs(gt);	// 오브젝트용
	UpdateMaterialCBs(gt);	// 머티리얼용
	UpdateMainPassCB(gt);	// 메인 패스용
}

void Chap8_1App::Draw(const GameTimer& gt)
{
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;
	ThrowIfFailed(cmdListAlloc->Reset());	// 커맨드 리스트 할당자 리셋
	ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mOpaquePSO.Get())); // 커맨드 리스트 리셋

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

	// 패스 상수 버퍼 설정
	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

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

void Chap8_1App::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
}

void Chap8_1App::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Chap8_1App::OnMouseMove(WPARAM btnState, int x, int y)
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
		float dx = 0.05f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.05f * static_cast<float>(y - mLastMousePos.y);

		mRadius += dx - dy;

		mRadius = MathHelper::Clamp(mRadius, 3.0f, 150.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Chap8_1App::OnKeyboardInput(const GameTimer& gt)
{
}

void Chap8_1App::UpdateCamera(const GameTimer& gt)
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

void Chap8_1App::AnimateMaterials(const GameTimer& gt)
{
}

void Chap8_1App::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for (auto& e : mAllRitems)
	{
		if (e->NumFramesDirty > 0)	// NumFramesDirty가 0보다 크면(변	화가 있으면) 업데이트
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstantsTex objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
			currObjectCB->CopyData(e->ObjCBIndex, objConstants);	// 상수 버퍼에 월드 행렬 전달
			e->NumFramesDirty--;	// NumFramesDirty 감소
		}
	}
}

void Chap8_1App::UpdateMaterialCBs(const GameTimer& gt)
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
			XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));
			currMaterialCB->CopyData(mat->MatCBIndex, matConstants);	// 상수 버퍼에 월드 행렬 전달
			e.second->NumFramesDirty--;	// NumFramesDirty 감소
		}
	}
}

void Chap8_1App::UpdateMainPassCB(const GameTimer& gt)
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

	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);	// 메인 패스 상수 버퍼에 전달
}

void Chap8_1App::BuildRootSignature()
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
	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0, 
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(), 
		IID_PPV_ARGS(mRootSignature.GetAddressOf()))
	);
}

void Chap8_1App::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		nullptr, nullptr
	};

	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void Chap8_1App::BuildShapeGeometry()
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
	std::vector<VertexLight> vertices(totalVertexCount);
	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
	}
	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
	}
	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
	}
	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
	}
	for (size_t i = 0; i < geoSphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = geoSphere.Vertices[i].Position;
		vertices[k].Normal = geoSphere.Vertices[i].Normal;
	}

	// 인덱스 버퍼 생성하고 채우기
	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));
	indices.insert(indices.end(), std::begin(geoSphere.GetIndices16()), std::end(geoSphere.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(VertexLight);
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

	geo->VertexByteStride = sizeof(VertexLight);	// 정점 하나의 크기 저장(GPU가 읽을 때 사용)
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

void Chap8_1App::BuildSkullGeometry()
{
}

void Chap8_1App::BuildPSOs()
{
}

void Chap8_1App::BuildFrameResources()
{
}

void Chap8_1App::BuildMaterials()
{
}

void Chap8_1App::BuildRenderItems()
{
}

void Chap8_1App::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp7*>& ritems)
{
}
