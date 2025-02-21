// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerMassComponent.h"

void USpawnerMassComponent::BeginPlay()
{
	Super::BeginPlay();

	// CurveInputGap(커브 입력의 간격) 미리 계산하기
	if (MassCount < 2)
	{
		CurveInputGap = 0.5f;	// 2개 이하일 경우 0.5로 설정
	}
	else
	{
		CurveInputGap = 1.0f / (float)(MassCount - 1);	// 2개 이상일 경우 1/(MassCount-1)로 설정 (0에서 시작 1에서 끝나기 위해)
	}
}

void USpawnerMassComponent::Spawn()
{
	// MassCount만큼 적 생성
	for (int32 i = 0; i < MassCount; i++)
	{
		FTransform SpawnTransform = GetSpawnTransform();	
		SpawnIndex++;	// 트랜스폼을 가져올 때마다 인덱스 증가
		GameMode->GetEnemy(SpawnEnemyType, SpawnTransform);
	}

	SpawnIndex = 0;		// 다시 초기화
}

FTransform USpawnerMassComponent::GetSpawnTransform() const
{
	FTransform Result = FTransform();
	if (SpawnArea && FormationCurve)
	{
		FVector LocaLocation;
		// X 결과는 0 ~ -Offset.X
		LocaLocation.X = (FormationCurve->GetFloatValue(CurveInputGap * SpawnIndex) * Offset.X - Offset.X);	
		// Y 결과는 -Offset.Y * 0.5 ~ Offset.Y * 0.5
		LocaLocation.Y = -Offset.Y * 0.5f + (CurveInputGap * SpawnIndex * Offset.Y);
		LocaLocation.Z = 0.0f;

		// SpawnArea의 Forward 방향을 기준으로 회전시키는 회전 만들기
		FRotator ForwardRotation = SpawnArea->GetForwardVector().ToOrientationRotator();
		LocaLocation = ForwardRotation.RotateVector(LocaLocation);	// 구했던 로컬 위치를 회전

		FVector BaseLocation = SpawnArea->GetComponentLocation();
		Result.SetLocation(BaseLocation + LocaLocation);	// SpawnArea 위치에 계산이 끝난 로컬 위치 더하기
		Result.SetRotation(SpawnArea->GetComponentQuat());
		Result.SetScale3D(SpawnArea->GetComponentScale());
	}

	return Result;
}
