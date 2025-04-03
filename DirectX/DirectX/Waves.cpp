#include "Waves.h"
#include <ppl.h>
#include <algorithm>
#include <cassert>

using namespace DirectX;

Waves::Waves(int m, int n, float dx, float dt, float speed, float damping)
{
	mNumRows = m;		// 가로 줄 수
	mNumCols = n;		// 세로 줄 수
	mVertexCount = m * n;
	mTriangleCount = (m - 1) * (n - 1) * 2;	// 가로세로줄로 만들어지는 사각형은 길이-1개만큼 생성됨

	mSpatialStep = dx;	// 공간 간격
	mTimeStep = dt;		// 시간 간격

	float d = damping * dt + 2.0f;		// 감쇠 계수
	float e = (speed * speed) * dt * dt / (dx * dx);	// 파동 속도
	mk1 = (damping * dt - 2.0f) / d;	// 감쇠 계수
	mk2 = (4.0f - 8.0f * e) / d;		// 파동 속도
	mk3 = (2.0f * e) / d;				// 파동 속도

	mPrevSolution.resize(m * n);	// 이전 파도 상태
	mCurrSolution.resize(m * n);	// 현재 파도 상태
	mNormals.resize(m * n);		// 법선 벡터
	mTangentX.resize(m * n);	// 접선 벡터

	float halfWidth = 0.5f * (n - 1) * dx;	// 가로 길이
	float halfDepth = 0.5f * (m - 1) * dx;	// 세로 길이

	// 그리드 정점 만들기
	for (int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;	// z좌표
		for (int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;	// x좌표

			mPrevSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);	// 이전 파도 상태 초기화
			mCurrSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);	// 현재 파도 상태 초기화
			mNormals[i * n + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);	// 법선 벡터 초기화
			mTangentX[i * n + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);	// 접선 벡터 초기화
		}
	}
}

Waves::~Waves()
{
}

void Waves::Update(float dt)
{
	static float t = 0.0f;	// 시간 누적용 변수

	t += dt;			// 시간 증가

	if (t >= mTimeStep) // 일정 시간 간격으로 업데이트
	{

		concurrency::parallel_for(1, mNumRows - 1, [this](int i)
		{
			for (int j = 1; j < mNumCols - 1; ++j)
			{
				mPrevSolution[i * mNumCols + j].y = 
					(mk1 * mPrevSolution[i * mNumCols + j].y) +
					(mk2 * mCurrSolution[i * mNumCols + j].y) +
					(mk3 * (mCurrSolution[(i + 1) * mNumCols + j].y +
					mCurrSolution[(i - 1) * mNumCols + j].y +
					mCurrSolution[i * mNumCols + (j + 1)].y +
					mCurrSolution[i * mNumCols + (j - 1)].y));
			}
		});

		std::swap(mPrevSolution, mCurrSolution);	// 이전 파도 상태와 현재 파도 상태 교환
		
		t = 0.0f;		// 시간 초기화

		// 법선 벡터 계산(정점의 앞뒤좌우에 있는 정점들의 높이 차이를 이용)
		concurrency::parallel_for(1, mNumRows - 1, [this](int i)
		{
			for (int j = 1; j < mNumCols - 1; ++j)
			{
				float l = mCurrSolution[i * mNumCols + (j - 1)].y;	// 왼쪽 정점 높이
				float r = mCurrSolution[i * mNumCols + (j + 1)].y;	// 오른쪽 정점 높이
				float t = mCurrSolution[(i - 1) * mNumCols + j].y;	// 위쪽 정점 높이
				float b = mCurrSolution[(i + 1) * mNumCols + j].y;	// 아래쪽 정점 높이
					
				mNormals[i * mNumCols + j].x = -r + l;	// 법선 벡터 x좌표
				mNormals[i * mNumCols + j].y = 2.0f * mSpatialStep;	// 법선 벡터 y좌표
				mNormals[i * mNumCols + j].z = b - t;	// 법선 벡터 z좌표

				XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&mNormals[i * mNumCols + j]));	// 법선 벡터 정규화
				XMStoreFloat3(&mNormals[i * mNumCols + j], n);	// 정규화한 결과를 다시 저장

				mTangentX[i * mNumCols + j] = XMFLOAT3(2.0f * mSpatialStep, r - l, 0.0f);	
				XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&mTangentX[i * mNumCols + j]));	// 접선 벡터 정규화
				XMStoreFloat3(&mTangentX[i * mNumCols + j], T);	// 정규화한 결과를 다시 저장

			}
		});
	}
}

void Waves::Disturb(int i, int j, float magnitude)
{
	// 경계부분에서는 생성되지 않음
	assert(i > 1 && i < mNumRows - 2);
	assert(j > 1 && j < mNumCols - 2);

	float halfMagnitude = 0.5f * magnitude;	// 중심부는 100%, 주변부는 50% 높이 증가
	
	mCurrSolution[i * mNumCols + j].y += magnitude;	// 현재 파도 상태 높이 증가

	mCurrSolution[i * mNumCols + j + 1].y += halfMagnitude;		// 오른쪽 정점 높이 증가
	mCurrSolution[i * mNumCols + j - 1].y += halfMagnitude;		// 왼쪽 정점 높이 증가
	mCurrSolution[(i + 1) * mNumCols + j].y += halfMagnitude;	// 아래쪽 정점 높이 증가
	mCurrSolution[(i - 1) * mNumCols + j].y += halfMagnitude;	// 위쪽 정점 높이 증가
}