#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount)
{
	ThrowIfFailed(
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
		)
	);

	PassCB = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);
}

FrameResource::~FrameResource()
{
}

FrameResourceWaves::FrameResourceWaves(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount)
{
	ThrowIfFailed(
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
		)
	);

	PassCB = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);

	WavesVB = std::make_unique<UploadBuffer<Vertex>>(device, waveVertCount, false);
}

FrameResourceWaves::~FrameResourceWaves()
{
}

FrameResourceMaterial::FrameResourceMaterial(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount)
{
	ThrowIfFailed(
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
		)
	);

	PassCB = std::make_unique<UploadBuffer<PassConstantsLight>>(device, passCount, true);
	MaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, materialCount, true);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstantsTex>>(device, objectCount, true);
}

FrameResourceMaterial::~FrameResourceMaterial()
{
}

FrameResourceMaterialWaves::FrameResourceMaterialWaves(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount, UINT waveVertCount)
{
	ThrowIfFailed(
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
		)
	);

	PassCB = std::make_unique<UploadBuffer<PassConstantsLight>>(device, passCount, true);
	MaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, materialCount, true);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);

	WavesVB = std::make_unique<UploadBuffer<VertexLight>>(device, waveVertCount, false);
}

FrameResourceMaterialWaves::~FrameResourceMaterialWaves()
{
}

FrameResourceTextureWaves::FrameResourceTextureWaves(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount, UINT waveVertCount)
{
	ThrowIfFailed(
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
		)
	);

	PassCB = std::make_unique<UploadBuffer<PassConstantsLight>>(device, passCount, true);
	MaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, materialCount, true);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstantsTex>>(device, objectCount, true);

	WavesVB = std::make_unique<UploadBuffer<VertexTex>>(device, waveVertCount, false);
}

FrameResourceTextureWaves::~FrameResourceTextureWaves()
{
}
