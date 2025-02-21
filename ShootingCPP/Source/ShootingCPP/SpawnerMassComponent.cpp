// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerMassComponent.h"

void USpawnerMassComponent::BeginPlay()
{
	Super::BeginPlay();

	// CurveInputGap 미리 계산하기
	if (MassCount < 2)
	{
		CurveInputGap = 0.5f;	// 2개 이하일 경우 0.5로 설정
	}
	else
	{
		CurveInputGap = 1.0f / (float)(MassCount - 1);	
	}
}

void USpawnerMassComponent::Spawn()
{
	for (int32 i = 0; i < MassCount; i++)
	{
		FTransform SpawnTransform = GetSpawnTransform();	
		SpawnIndex++;
		GameMode->GetEnemy(SpawnEnemyType, SpawnTransform);
	}

	SpawnIndex = 0;
}

FTransform USpawnerMassComponent::GetSpawnTransform() const
{
	FTransform Result = FTransform();
	if (SpawnArea)
	{
		FVector SpawnLocation;
		SpawnLocation.X = (FormationCurve->GetFloatValue(CurveInputGap * SpawnIndex) * Offset.X - Offset.X);
		SpawnLocation.Y = (CurveInputGap * SpawnIndex * Offset.Y);
		SpawnLocation.Z = 0.0f;

		FRotator ForwardRotation = SpawnArea->GetForwardVector().ToOrientationRotator();
		SpawnLocation = ForwardRotation.RotateVector(SpawnLocation);

		FVector BaseLocation = SpawnArea->GetComponentLocation();
		Result.SetLocation(BaseLocation + SpawnLocation);
		Result.SetRotation(SpawnArea->GetComponentQuat());
		Result.SetScale3D(SpawnArea->GetComponentScale());
	}

	return Result;
}
