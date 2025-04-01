#pragma once
#include "./Common/d3dApp.h"
#include "./Common/MathHelper.h"
#include "./Common/UploadBuffer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;		// 정점의 위치
	XMFLOAT4 Color;		// 정점의 색상
};

struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();	// 월드, 뷰, 투영 행렬
};

class Chap6App : public D3DApp
{
public:
	Chap6App(HINSTANCE hInstance);
	virtual ~Chap6App();
	Chap6App(const Chap6App& rhs) = delete;
	Chap6App& operator=(const Chap6App& rhs) = delete;

	virtual bool Initialize() override;

protected:
	virtual void OnResize() override;
	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

private:
	void BuildDescriptorHeaps();		// 디스크립터 힙 생성
	void BuildConstantBuffers();		// 상수 버퍼 생성
	void BuildRootSignature();			// 루트 시그니처 생성
	void BuildShadersAndInputLayout();	// 셰이더와 입력 레이아웃 생성
	void BuildBoxGeometry();			// 정점과 인덱스 생성
	void BuildPSO();					// 파이프라인 상태 객체 생성

private:
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;	// 루트 시그니처
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;		// 디스크립터 힙
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;	// 상수 버퍼
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;		// 상자 메쉬
	ComPtr<ID3DBlob> mvsByteCode = nullptr;					// 정점 셰이더 바이트 코드
	ComPtr<ID3DBlob> mpsByteCode = nullptr;					// 픽셀 셰이더 바이트 코드
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;		// 입력 레이아웃
	ComPtr<ID3D12PipelineState> mPSO = nullptr;				// 파이프라인 상태 객체
	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();			// 월드 행렬
	XMFLOAT4X4 mView = MathHelper::Identity4x4();			// 뷰 행렬
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();			// 투영 행렬
	float mTheta = 1.5f * XM_PI;							// 카메라 x축 회전
	float mPhi = XM_PIDIV4;									// 카메라 y축 회전
	float mRadius = 5.0f;									// 카메라 거리
	POINT mLastMousePos;									// 마우스 위치
};

