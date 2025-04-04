#pragma once
#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/GeometryGenerator.h"
#include "FrameResource.h"
#include "RenderItem.h"

class Chap9_1App : public D3DApp
{
public:
	Chap9_1App(HINSTANCE hInstance);
	virtual ~Chap9_1App();
	Chap9_1App(const Chap9_1App& rhs) = delete;
	Chap9_1App& operator=(const Chap9_1App& rhs) = delete;

	virtual bool Initialize() override;

protected:
	virtual void OnResize() override;
	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

private:
	void OnKeyboardInput(const GameTimer& gt);		// 키보드 입력 처리
	void UpdateCamera(const GameTimer& gt);			// 카메라 업데이트
	void AnimateMaterials(const GameTimer& gt);		// 머티리얼 애니메이션 업데이트
	void UpdateObjectCBs(const GameTimer& gt);		// 오브젝트 상수 버퍼 업데이트
	void UpdateMaterialCBs(const GameTimer& gt);	// 머티리얼 상수 버퍼 업데이트
	void UpdateMainPassCB(const GameTimer& gt);		// 메인 패스 상수 버퍼 업데이트

	void LoadTextures();				// 텍스쳐 로드
	void BuildRootSignature();			// 루트 시그니처 생성
	void BuildDescriptorHeaps();		// 디스크립터 힙 생성
	void BuildShadersAndInputLayout();	// 셰이더와 입력 레이아웃 생성
	void BuildShapeGeometry();			// 정점과 인덱스 생성(박스, 구, 원뿔)
	void BuildPSOs();					// 파이프라인 상태 객체 생성
	void BuildFrameResources();			// 프레임 리소스 생성
	void BuildMaterials();				// 머티리얼 생성
	void BuildRenderItems();			// 랜더 아이템 생성
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp9*>& ritems);	// 랜더 아이템 그리기

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();	// 정적 샘플러 생성

private:
	std::vector<std::unique_ptr<FrameResourceMaterial>> mFrameResources;	// 프레임 리소스
	FrameResourceMaterial* mCurrFrameResource = nullptr;					// 현재 프레임 리소스
	int mCurrFrameResourceIndex = 0;								// 현재 프레임 자원 인덱스

	UINT mCbvSrvDescriptorSize = 0;									// CBV/SRV 디스크립터 크기

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;			// 루트 시그니처

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;		// 디스크립터 힙(쉐이더 리소스 뷰 용)

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;	// 지오메트리 맵
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;		// 머티리얼 맵
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;		// 텍스쳐 맵
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;					// 셰이더 맵

	ComPtr<ID3D12PipelineState> mOpaquePSO;			// 파이프라인 상태 객체 맵(불투명)

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;	// 입력 레이아웃

	std::vector<std::unique_ptr<RenderItemApp9>> mAllRitems;	// 모든 랜더 아이템

	std::vector<RenderItemApp9*> mOpaqueRitems;		// 불투명 랜더 아이템

	PassConstantsLight mMainPassCB;		// 메인 패스 상수 버퍼

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };		// 뷰어의 위치
	XMFLOAT4X4 mView = MathHelper::Identity4x4();	// 뷰 행렬
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();	// 투영 행렬
	float mTheta = 1.3f * XM_PI;	// 뷰어의 수평 각도
	float mPhi = 0.4f * XM_PI;		// 뷰어의 수직 각도
	float mRadius = 2.0f;			// 뷰어와 대상 사이의 거리

	POINT mLastMousePos;	// 마우스 위치
};

