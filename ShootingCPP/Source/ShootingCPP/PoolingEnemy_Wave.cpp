// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_Wave.h"

APoolingEnemy_Wave::APoolingEnemy_Wave()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolingEnemy_Wave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WaveTimeline.TickTimeline(DeltaTime);	// 타임라인을 업데이트
}

void APoolingEnemy_Wave::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat WaveCallback;
	WaveCallback.BindUFunction(this, FName("UpdateWave"));
	WaveTimeline.AddInterpFloat(WaveCurve, WaveCallback);	// 타임라인에 콜백함수와 커브를 바인딩
	WaveTimeline.SetLooping(true);				// 계속 반복	
	WaveTimeline.SetPlayRate(WaveSpeedRate);	// 재생 속도 변경
}

void APoolingEnemy_Wave::OnActivate()
{
	Super::OnActivate();
	SetActorTickEnabled(true);		// 틱 활성화
	WaveTimeline.PlayFromStart();	// 처음부터 시작
}

void APoolingEnemy_Wave::OnDeactivate()
{
	Super::OnDeactivate();
	WaveTimeline.Stop();			// 타임라인 정지
	SetActorTickEnabled(false);		// 틱 비활성화
}

void APoolingEnemy_Wave::UpdateWave(float value)
{
	FVector NewLocation(0, WaveWidth * value, 0);	// 오른쪽 축 위를 계속 이동
	StaticMesh->SetRelativeLocation(NewLocation);
}
