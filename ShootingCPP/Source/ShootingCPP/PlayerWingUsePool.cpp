// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWingUsePool.h"


APlayerWingUsePool::APlayerWingUsePool()
{
	PrimaryActorTick.bCanEverTick = true;
	NormalProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("NormalProjectilePool"));
	HomingProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("HomingProjectilePool"));
	AreaProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("AreaProjectilePool"));
}

void APlayerWingUsePool::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerWingUsePool::OnFireStart(EProjectileType Type)
{
	switch (Type)
	{
	case APlayerWing::EProjectileType::Normal:
		NormalProjectilePool->GetObject(GetFireTransform());
		break;
	case APlayerWing::EProjectileType::Homming:
		break;
	case APlayerWing::EProjectileType::Area:
		break;
	default:
		break;
	}
}

void APlayerWingUsePool::OnFireEnd(EProjectileType Type)
{
}

const FTransform APlayerWingUsePool::GetFireTransform() const
{
	static bool bFireLocationSelector = false;

	if (bFireLocationSelector)
	{
		bFireLocationSelector = false;
		return StaticMesh->GetSocketTransform(TEXT("FireSocket2"), ERelativeTransformSpace::RTS_World);
	}
	else
	{
		bFireLocationSelector = true;
		return StaticMesh->GetSocketTransform(TEXT("FireSocket1"), ERelativeTransformSpace::RTS_World);
	}
}
