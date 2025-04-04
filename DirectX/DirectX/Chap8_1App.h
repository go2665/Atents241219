#pragma once
#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/GeometryGenerator.h"
#include "FrameResource.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

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

class Chap8_1App : public D3DApp
{
public:
	Chap8_1App(HINSTANCE hInstance);
	virtual ~Chap8_1App();
	Chap8_1App(const Chap8_1App& rhs) = delete;
	Chap8_1App& operator=(const Chap8_1App& rhs) = delete;

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

	void BuildRootSignature();			// 루트 시그니처 생성
	void BuildShadersAndInputLayout();	// 셰이더와 입력 레이아웃 생성
	void BuildShapeGeometry();			// 정점과 인덱스 생성(박스, 구, 원뿔)
	void BuildSkullGeometry();			// 스컬 지오메트리 생성
	void BuildPSOs();					// 파이프라인 상태 객체 생성
	void BuildFrameResources();			// 프레임 리소스 생성
	void BuildMaterials();				// 머티리얼 생성
	void BuildRenderItems();			// 랜더 아이템 생성
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp8*>& ritems);	// 랜더 아이템 그리기

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

	std::vector<std::unique_ptr<RenderItemApp8>> mAllRitems;	// 모든 랜더 아이템

	std::vector<RenderItemApp8*> mOpaqueRitems;		// 불투명 랜더 아이템

	PassConstantsLight mMainPassCB;		// 메인 패스 상수 버퍼

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };		// 뷰어의 위치
	XMFLOAT4X4 mView = MathHelper::Identity4x4();	// 뷰 행렬
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();	// 투영 행렬
	float mTheta = 1.5f * XM_PI;	// 뷰어의 수평 각도
	float mPhi = XM_PIDIV4;			// 뷰어의 수직 각도
	float mRadius = 15.0f;			// 뷰어와 대상 사이의 거리

	POINT mLastMousePos;	// 마우스 위치
};

