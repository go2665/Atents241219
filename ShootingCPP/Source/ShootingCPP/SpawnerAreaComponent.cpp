// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerAreaComponent.h"

FTransform USpawnerAreaComponent::GetSpawnTransform() const
{
	FTransform Result;
	
	FVector Origin = SpawnArea->Bounds.Origin;		// 스폰 영역의 중심
	FVector Extend = SpawnArea->Bounds.BoxExtent;	// 스폰 영역의 각 변의 절반

	Result.SetLocation(FVector(
		FMath::FRandRange(Origin.X - Extend.X, Origin.X + Extend.X),
		FMath::FRandRange(Origin.Y - Extend.Y, Origin.Y + Extend.Y),
		Origin.Z));		// 높이는 그대로 유지하고 X, Y는 랜덤으로 설정
	Result.SetRotation(SpawnArea->GetComponentQuat());	// 로테이션과 스케일은 그대로 유지
	Result.SetScale3D(SpawnArea->GetComponentScale());	

	return Result;
}
