// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWingUsePool.h"


APlayerWingUsePool::APlayerWingUsePool()
{
	PrimaryActorTick.bCanEverTick = true;
	NormalProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("NormalProjectilePool"));
	HomingProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("HomingProjectilePool"));
	AreaProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("AreaProjectilePool"));
}

void APlayerWingUsePool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireCoolTime_Normal -= DeltaTime;
	FireCoolTime_Homing -= DeltaTime;
	FireCoolTime_Area -= DeltaTime;
}

void APlayerWingUsePool::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerWingUsePool::OnFireStart(EProjectileType Type)
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	switch (Type)
	{
	case APlayerWing::EProjectileType::Normal:
		if (FireCoolTime_Normal < 0)
		{
			NormalProjectilePool->GetObject(GetFireTransform());
			FireCoolTime_Normal = Interval_Normal;
			TimerManager.SetTimer(TimerHandle_Normal, 
				[this](){
					FireCoolTime_Normal = Interval_Normal;
					NormalProjectilePool->GetObject(GetFireTransform()); 
				},
				Interval_Normal, true);		
		}
		break;
	case APlayerWing::EProjectileType::Homing:
		if (FireCoolTime_Homing < 0)
		{
			HomingProjectilePool->GetObject(GetActorTransform());
			FireCoolTime_Homing = Interval_Homing;			
			TimerManager.SetTimer(TimerHandle_Homing,
				[this]() {
					FireCoolTime_Homing = Interval_Homing;
					HomingProjectilePool->GetObject(GetActorTransform());
				},
				Interval_Homing, true);
		}
		break;
	case APlayerWing::EProjectileType::Area:
		if (FireCoolTime_Area < 0)
		{			
			AreaProjectilePool->GetObject(GetActorTransform());
			FireCoolTime_Area = Interval_Area;
			TimerManager.SetTimer(TimerHandle_Area,
				[this]() {
					FireCoolTime_Area = Interval_Area;
					AreaProjectilePool->GetObject(GetActorTransform());
				},
				Interval_Area, true);
		}
		break;
	default:
		break;
	}
}

void APlayerWingUsePool::OnFireEnd(EProjectileType Type)
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	switch (Type)
	{
	case APlayerWing::EProjectileType::Normal:
		TimerManager.ClearTimer(TimerHandle_Normal);
		TimerHandle_Normal.Invalidate();
		break;
	case APlayerWing::EProjectileType::Homing:
		TimerManager.ClearTimer(TimerHandle_Homing);
		TimerHandle_Homing.Invalidate();
		break;
	case APlayerWing::EProjectileType::Area:
		TimerManager.ClearTimer(TimerHandle_Area);
		TimerHandle_Area.Invalidate();
		break;
	default:
		break;
	}
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
