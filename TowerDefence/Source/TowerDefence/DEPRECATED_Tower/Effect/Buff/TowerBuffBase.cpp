// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuffBase.h"
#include "TowerDefence/DEPRECATED_Tower/TowerBaseActor.h"

void UTowerBuffBase::OnInitialize(const ATowerBaseActor* InTargetTower, const UTowerBuffDataAsset* InData)
{
	BuffDataAsset = InData;
	TargetTower = InTargetTower;
}

void UTowerBuffBase::OnBuffBegin()
{
	// 버프가 시작 되었을 때 할일
	CurrentDuration = BuffDataAsset->Duration;
	// 이 버프가 처음 적용된 타워의 상태를 변경하는 로직을 여기에 추가합니다.
}

void UTowerBuffBase::OnBuffTick(float DeltaTime)
{
	// 버프 틱별 개별 적용
	CurrentDuration -= DeltaTime;
	// 이 버프가 적용된 타워의 상태를 매 프레임마다 변경해야 할 경우, 해당하는 로직을 여기에 추가합니다.
}

void UTowerBuffBase::OnBuffEnd()
{
	// 이 버프가 끝났을 때 타워의 상태를 원래대로 되돌리는 로직을 여기에 추가합니다.
	CurrentDuration = 0.0f;
}
