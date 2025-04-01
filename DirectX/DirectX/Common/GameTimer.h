//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop. 타이머 초기화 및 리셋
	void Start(); // Call when unpaused. 타이머 일시 정지 해제
	void Stop();  // Call when paused. 타이머 일시 정지
	void Tick();  // Call every frame. 매 프레임마다 호출

private:
	double mSecondsPerCount;	// 클럭 카운트당 시간(초)
	double mDeltaTime;			// 이전 프레임과 현재 프레임의 시간 차이(초)

	__int64 mBaseTime;		// Reset되었을 때의 클럭 카운트
	__int64 mPausedTime;	// 정지되어 있던 클럭 카운트의 합
	__int64 mStopTime;		// 정지된 타이밍의 클럭 카운트(정지 되어있을 때만 유효)
	__int64 mPrevTime;		// 이전 프레임의 클럭 카운트
	__int64 mCurrTime;		// 현재 프레임의 클럭 카운트

	bool mStopped;			// 타이머 일시정지 여부
};

#endif // GAMETIMER_H