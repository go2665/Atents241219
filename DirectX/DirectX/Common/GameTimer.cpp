//***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), 
  mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;	// 카운트당 시간(초)
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
// Reset함수가 호출된 이후의 총 경과시간을 반환한다. 단 정지되어 있던 시간은 제외한다.
float GameTimer::TotalTime()const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if( mStopped )	// 현재 정지 상태라면
	{
		// (정지된클럭카운트-누적된정지카운트-시작클럭카운트) * 카운트당 시간
		return (float)(((mStopTime - mPausedTime)-mBaseTime)*mSecondsPerCount);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
	
	else
	{
		// (현재클럭카운트-정지되어있던클럭카운트-시작클럭카운트) * 카운트당 시간
		return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
	}
}

float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	// 현재 CPU의 클럭 카운트를 가져온다.

	mBaseTime = currTime;	// mBaseTime = Reset되었을 때의 클럭 카운트
	mPrevTime = currTime;	// mPrevTime = 이전 프레임의 클럭 카운트
	mStopTime = 0;			// mStopTime = 정지되어 있던 클럭 카운트 초기화
	mStopped = false;		// 정지하고 있지 않다고 표시
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	if( mStopped )
	{
		mPausedTime += (startTime - mStopTime);	// 정지되어 있던 시간을 누적한다.

		mPrevTime = startTime;	// 다시 시작을 위해 mPrevTime를 현재 프레임카운트로 설정
		mStopTime = 0;			// 정지시간 초기화
		mStopped = false;		// 정지상태 해제
	}
}

void GameTimer::Stop()
{
	if (!mStopped)	// 정지되어 있지 않다면 정지
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;	// Stop한 클럭 카운트 저장
		mStopped = true;		// 정지 상태로 변경
	}
}

void GameTimer::Tick()
{
	if( mStopped )
	{
		mDeltaTime = 0.0;	// 일시 정지 상태일때는 시간이 흐르지 않는다.(델타타임도 0)
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;	// 현재 클럭 카운트 저장

	// Time difference between this frame and the previous.
	// 델타타임 = (현재프레임의 클럭카운트 - 이전프레임의 클럭카운트) * 카운트당 시간
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;	

	// Prepare for next frame. 계산이 끝났으니 현제 프레임을 이전 프레임으로 설정.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if(mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;	// 델타타임이 음수가 되는 것 방지
	}
}

