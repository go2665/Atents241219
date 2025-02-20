// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerAreaComponent.h"

FTransform USpawnerAreaComponent::GetSpawnTransform() const
{
	FTransform Result;
	
	FVector Origin = SpawnArea->Bounds.Origin;
	FVector Extend = SpawnArea->Bounds.BoxExtent;

	Result.SetLocation(FVector(
		FMath::FRandRange(Origin.X - Extend.X, Origin.X + Extend.X),
		FMath::FRandRange(Origin.Y - Extend.Y, Origin.Y + Extend.Y),
		Origin.Z));
	Result.SetRotation(SpawnArea->GetComponentQuat());
	Result.SetScale3D(SpawnArea->GetComponentScale());	

	return Result;
}
