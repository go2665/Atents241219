#pragma once
#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/GeometryGenerator.h"
#include "FrameResource.h"
#include "RenderItem.h"

class Chap7_1App : public D3DApp
{
public:
	Chap7_1App(HINSTANCE hInstance);
	virtual ~Chap7_1App();
	Chap7_1App(const Chap7_1App& rhs) = delete;
	Chap7_1App& operator=(const Chap7_1App& rhs) = delete;
	
	virtual bool Initialize() override;

protected:
	virtual void OnResize() override;
	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;
	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

private:
	void OnKeyboardInput(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

	void BuildDescriptorHeaps();		// 디스크립터 힙 생성
	void BuildConstantBufferViews();	// 상수 버퍼 뷰 생성
	void BuildRootSignature();			// 루트 시그니처 생성
	void BuildShadersAndInputLayout();	// 셰이더와 입력 레이아웃 생성
	void BuildShapeGeometry();			// 정점과 인덱스 생성(박스, 구, 원뿔)
	void BuildPSOs();					// 파이프라인 상태 객체 생성
	void BuildFrameResources();			// 프레임 리소스 생성
	void BuildRenderItems();			// 랜더 아이템 생성
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp7*>& ritems);	// 랜더 아이템 그리기

private:
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;	// 프레임 리소스
	FrameResource* mCurrFrameResource = nullptr;					// 현재 프레임 리소스
	int mCurrFrameResourceIndex = 0;								// 현재 프레임 자원 인덱스
	
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;			// 루트 시그니처
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;				// 디스크립터 힙
	
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;	// 지오메트리 맵
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;					// 셰이더 맵
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;			// 파이프라인 상태 객체 맵

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;	// 입력 레이아웃

	std::vector<std::unique_ptr<RenderItemApp7>> mAllRitems;	// 모든 랜더 아이템
	std::vector<RenderItemApp7*> mOpaqueRitems;					

	PassConstants mMainPassCB;		// 메인 패스 상수 버퍼
	UINT mPassCbvOffset = 0;		// 패스 상수 버퍼 오프셋

	bool mIsWireframe = false;		// 와이어프레임 모드

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };	// 뷰어의 위치
	XMFLOAT4X4 mView = MathHelper::Identity4x4();	// 뷰 행렬
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();	// 투영 행렬
	float mTheta = 1.5f * XM_PI;	// 뷰어의 수평 각도
	float mPhi = XM_PIDIV4;			// 뷰어의 수직 각도
	float mRadius = 15.0f;			// 뷰어와 대상 사이의 거리

	POINT mLastMousePos;	// 마우스 위치
};

