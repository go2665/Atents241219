#pragma once
#include <vector>
#include <DirectXMath.h>

class Waves
{
public:
	Waves(int m, int n, float dx, float dt, float speed, float damping);
	Waves(const Waves& rhs) = delete;
	Waves& operator=(const Waves& rhs) = delete;
	~Waves();

	// 파도 업데이트
	void Update(float dt);

	// 파도 생성
	void Disturb(int i, int j, float magnitude);

	inline const DirectX::XMFLOAT3& Position(int i) const { return mCurrSolution[i]; }
	inline const DirectX::XMFLOAT3& Normal(int i) const { return mNormals[i]; };
	inline const DirectX::XMFLOAT3& TangentX(int i) const { return mTangentX[i]; };

	inline int RowCount() const { return mNumRows; }
	inline int ColumnCount() const { return mNumCols; }
	inline int VertexCount() const { return mNumRows * mNumCols; }
	inline int TriangleCount() const { return mTriangleCount; }
	inline float Width() const { return mNumCols * mSpatialStep; }
	inline float Depth() const { return mNumRows * mSpatialStep; }	

private:
	int mNumRows = 0;		// 행 개수
	int mNumCols = 0;		// 열 개수

	int mVertexCount = 0;	// 정점 개수
	int mTriangleCount = 0;	// 삼각형 개수

	float mk1 = 0.0f;	
	float mk2 = 0.0f;	
	float mk3 = 0.0f;	

	float mTimeStep = 0.0f;		// 시간 간격
	float mSpatialStep = 0.0f;	// 공간 간격

	std::vector<DirectX::XMFLOAT3> mPrevSolution;	// 이전 파도 상태
	std::vector<DirectX::XMFLOAT3> mCurrSolution;	// 현재 파도 상태
	std::vector<DirectX::XMFLOAT3> mNormals;
	std::vector<DirectX::XMFLOAT3> mTangentX;

};

