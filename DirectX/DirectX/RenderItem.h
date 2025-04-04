#pragma once
#include "Common/d3dUtil.h"    

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;


// 쉐이프를 그릴때 필요한 파라메터를 저장하고 있는 경량 구조체(App마다 다를 수 있음)
struct RenderItemApp7
{
	RenderItemApp7() = default;

	// 월드 행렬
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	int NumFramesDirty = gNumFrameResources;	 // 갱신여부를 나타낼 플래그

	// 이 랜더 아이템의 오브젝트 상수버퍼의 인덱스
	UINT ObjCBIndex = -1;

	// 그릴 쉐이프의 지오메트리
	MeshGeometry* Geo = nullptr;

	// 프리미티브 타입
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;			// 그릴 쉐이프의 인덱스 버퍼의 인덱스 개수
	UINT StartIndexLocation = 0;	// 그릴 쉐이프의 인덱스 버퍼의 시작 인덱스
	int BaseVertexLocation = 0;		// 그릴 쉐이프의 정점 버퍼의 시작 인덱스

};

struct RenderItemApp8
{
	RenderItemApp8() = default;

	// 월드 행렬
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();	// 텍스쳐 변환 행렬

	int NumFramesDirty = gNumFrameResources;	 // 갱신여부를 나타낼 플래그

	// 이 랜더 아이템의 오브젝트 상수버퍼의 인덱스
	UINT ObjCBIndex = -1;

	// 머티리얼 주소
	Material* Mat = nullptr;

	// 그릴 쉐이프의 지오메트리
	MeshGeometry* Geo = nullptr;

	// 프리미티브 타입
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;			// 그릴 쉐이프의 인덱스 버퍼의 인덱스 개수
	UINT StartIndexLocation = 0;	// 그릴 쉐이프의 인덱스 버퍼의 시작 인덱스
	int BaseVertexLocation = 0;		// 그릴 쉐이프의 정점 버퍼의 시작 인덱스
};

struct RenderItemApp9
{
	RenderItemApp9() = default;

	// 월드 행렬
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();	// 텍스쳐 변환 행렬

	int NumFramesDirty = gNumFrameResources;	 // 갱신여부를 나타낼 플래그

	// 이 랜더 아이템의 오브젝트 상수버퍼의 인덱스
	UINT ObjCBIndex = -1;

	// 머티리얼 주소
	Material* Mat = nullptr;

	// 그릴 쉐이프의 지오메트리
	MeshGeometry* Geo = nullptr;

	// 프리미티브 타입
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;			// 그릴 쉐이프의 인덱스 버퍼의 인덱스 개수
	UINT StartIndexLocation = 0;	// 그릴 쉐이프의 인덱스 버퍼의 시작 인덱스
	int BaseVertexLocation = 0;		// 그릴 쉐이프의 정점 버퍼의 시작 인덱스
};