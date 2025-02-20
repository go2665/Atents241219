// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerDiagonalComponent.h"

void USpawnerDiagonalComponent::BeginPlay()
{
	if (SpawnInterval < SubSpawnInterval * SubSpawnCount)
	{
		SpawnInterval = SubSpawnInterval * SubSpawnCount + 1.0f;
	}
	Super::BeginPlay();
}

void USpawnerDiagonalComponent::Spawn()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(
		SubSpawnTimer,
		this, &USpawnerDiagonalComponent::SubSpawn,
		SubSpawnInterval, true);
}

FTransform USpawnerDiagonalComponent::GetSpawnTransform() const
{
	FTransform Result = FTransform();
	if (SpawnArea)
	{
		FVector BaseLocation = SpawnArea->GetComponentLocation();
		FVector Right = SpawnArea->GetRightVector();
		FVector SpawnLocation = BaseLocation + SubSpawnSpacing * SubSpawnIndex * Right;
		
		Result.SetLocation(SpawnLocation);
		Result.SetRotation(SpawnArea->GetComponentQuat());
		Result.SetScale3D(SpawnArea->GetComponentScale());
	}
	
	return Result;
}

void USpawnerDiagonalComponent::SubSpawn()
{
	GameMode->GetEnemy(SpawnEnemyType, GetSpawnTransform());
	SubSpawnIndex++;

	if (SubSpawnIndex >= SubSpawnCount)
	{
		SubSpawnIndex = 0;
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(SubSpawnTimer);
		SubSpawnTimer.Invalidate();
	}
}
