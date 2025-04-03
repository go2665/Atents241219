#pragma once
#include "Common/d3dUtil.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
};

struct ObjectConstantsTex
{
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
};

struct PassConstants
{
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
};

struct PassConstantsLight
{
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;

	DirectX::XMFLOAT4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f };	// 환경광(Ambient Light)

	Light Lights[MaxLights];	// 디렉셔널, 포인트, 스포트 라이트
};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct VertexLight
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
};

// CPU가 각 프레임의 커맨드 리스트를 만드는데 필요한 리소스들을 저장하는 구조체
struct FrameResource
{
public:
	FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount);
	FrameResource(const FrameResource& rhs) = delete;
	FrameResource& operator=(const FrameResource& rhs) = delete;
	~FrameResource();

	// 각 프레임별로 커맨드 리스트 할당자를 가짐(GPU가 커맨드를 완료해야 리셋할 수 있기 때문에 따로 가짐)
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc = nullptr;

	// 상수 버퍼(상수퍼퍼를 업데이트 하려면 GPU가 커맨드를 완료해야 하기 때문에 별도로 가짐)
	std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

	// 프레임 번호
	UINT64 Fence = 0;
};

struct FrameResourceWaves
{
public:
	FrameResourceWaves(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount);
	FrameResourceWaves(const FrameResourceWaves& rhs) = delete;
	FrameResourceWaves& operator=(const FrameResourceWaves& rhs) = delete;
	~FrameResourceWaves();

	// 각 프레임별로 커맨드 리스트 할당자를 가짐(GPU가 커맨드를 완료해야 리셋할 수 있기 때문에 따로 가짐)
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc = nullptr;

	// 상수 버퍼(상수버퍼를 업데이트 하려면 GPU가 커맨드를 완료해야 하기 때문에 별도로 가짐)
	std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

	// 파도 정점 버퍼
	std::unique_ptr<UploadBuffer<Vertex>> WavesVB = nullptr;

	// 프레임 번호
	UINT64 Fence = 0;
};

struct FrameResourceMaterial
{
public:
	FrameResourceMaterial(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount);
	FrameResourceMaterial(const FrameResourceMaterial& rhs) = delete;
	FrameResourceMaterial& operator=(const FrameResourceMaterial& rhs) = delete;
	~FrameResourceMaterial();

	// 각 프레임별로 커맨드 리스트 할당자를 가짐(GPU가 커맨드를 완료해야 리셋할 수 있기 때문에 따로 가짐)
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc = nullptr;

	// 상수 버퍼(상수퍼퍼를 업데이트 하려면 GPU가 커맨드를 완료해야 하기 때문에 별도로 가짐)
	std::unique_ptr<UploadBuffer<PassConstantsLight>> PassCB = nullptr;
	std::unique_ptr<UploadBuffer<MaterialConstants>> MaterialCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstantsTex>> ObjectCB = nullptr;

	// 프레임 번호
	UINT64 Fence = 0;
};