#pragma once
#include "Chap9_1App.h"
#include "Waves.h"

class Chap9_2App : public D3DApp
{
	enum class RenderLayer : int
	{
		Opaque = 0,
		Count
	};

public:
	Chap9_2App(HINSTANCE hInstance);
	Chap9_2App(const Chap9_2App& rhs) = delete;
	Chap9_2App& operator=(const Chap9_2App& rhs) = delete;
	~Chap9_2App();

	virtual bool Initialize()override;

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
	void AnimateMaterials(const GameTimer& gt);			// 머티리얼 애니메이션
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
	void UpdateWaves(const GameTimer& gt);

	void LoadTextures();				// 텍스쳐 로드
	void BuildRootSignature();			// 루트 시그니처 생성
	void BuildDescriptorHeaps();		// 디스크립터 힙 생성
	void BuildShadersAndInputLayout();	// 셰이더와 입력 레이아웃 생성
	void BuildLandGeometry();			// 정점과 인덱스 생성(지형)
	void BuildWavesGeometry();	// 정점과 인덱스 생성(파도)
	void BuildBoxGeometry();			// 정점과 인덱스 생성(박스)
	void BuildPSOs();					// 파이프라인 상태 객체 생성
	void BuildFrameResources();			// 프레임 리소스 생성
	void BuildMaterials();				// 머티리얼 생성	
	void BuildRenderItems();			// 랜더 아이템 생성
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp9*>& ritems);	// 랜더 아이템 그리기

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();	// 정적 샘플러 생성

	float GetHillsHeight(float x, float z) const;		// 지형 높이 계산
	XMFLOAT3 GetHillsNormal(float x, float z) const;	// 지형 법선 계산

private:
	std::vector<std::unique_ptr<FrameResourceTextureWaves>> mFrameResources;	// 프레임 리소스
	FrameResourceTextureWaves* mCurrFrameResource = nullptr;					// 현재 프레임 리소스
	int mCurrFrameResourceIndex = 0;									// 현재 프레임 자원 인덱스

	UINT mCbvSrvDescriptorSize = 0;		// CBV/SRV 디스크립터 크기

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;			// 루트 시그니처

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;		// SRV 디스크립터 힙

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;	// 지오메트리 맵
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;		// 머티리얼 맵
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;		// 텍스쳐 맵
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;					// 셰이더 맵
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;			// 파이프라인 상태 객체 맵

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;	// 입력 레이아웃

	RenderItemApp9* mWavesRitem = nullptr;				// 파도 랜더 아이템(정점 변경 때문에 따로 저장)

	std::vector<std::unique_ptr<RenderItemApp9>> mAllRitems;	// 모든 랜더 아이템

	std::vector<RenderItemApp9*> mRitemLayer[static_cast<int>(RenderLayer::Count)];	// 랜더 아이템 레이어

	std::unique_ptr<Waves> mWaves = nullptr;			// 파도 객체

	PassConstantsLight mMainPassCB;		// 메인 패스 상수 버퍼

	bool mIsWireframe = false;		// 와이어프레임 모드

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };	// 뷰어의 위치
	XMFLOAT4X4 mView = MathHelper::Identity4x4();	// 뷰 행렬
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();	// 투영 행렬
	float mTheta = 1.5f * XM_PI;	// 뷰어의 수평 각도
	float mPhi = XM_PIDIV4;			// 뷰어의 수직 각도
	float mRadius = 15.0f;			// 뷰어와 대상 사이의 거리

	float mSunTheta = 1.25f * XM_PI;	// 태양의 수평 각도
	float mSunPhi = XM_PIDIV4;			// 태양의 수직 각도

	POINT mLastMousePos;	// 마우스 위치
};

