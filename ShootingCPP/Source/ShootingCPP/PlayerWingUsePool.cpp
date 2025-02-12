// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWingUsePool.h"


APlayerWingUsePool::APlayerWingUsePool()
{
	PrimaryActorTick.bCanEverTick = true;

	// 발사체들의 풀 컴포넌트 생성
	NormalProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("NormalProjectilePool"));
	HomingProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("HomingProjectilePool"));
	AreaProjectilePool = CreateDefaultSubobject<UObjectPoolActorComponent>(TEXT("AreaProjectilePool"));
}

void APlayerWingUsePool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 발사체 쿨타임은 계속 감소
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
		if (IsFireReady_Normal())	// 발사 쿨타임이 다 되었으면
		{
			FireCoolTime_Normal = Interval_Normal;					// 발사 쿨타임 초기화	
			NormalProjectilePool->GetObject(GetFireTransform());	// 발사체 풀에서 발사체 가져와서 발사
			TimerManager.SetTimer(TimerHandle_Normal,				// 타이머로 주기적으로 발사하게 만들기
				[this](){
					FireCoolTime_Normal = Interval_Normal;
					NormalProjectilePool->GetObject(GetFireTransform()); 
				},
				Interval_Normal, true);								// Interval_Normal 간격으로 계속 발사
		}
		break;
	case APlayerWing::EProjectileType::Homing:
		if (IsFireReady_Homing())
		{
			FireCoolTime_Homing = Interval_Homing;			
			HomingProjectilePool->GetObject(GetActorTransform());
			TimerManager.SetTimer(TimerHandle_Homing,
				[this]() {
					FireCoolTime_Homing = Interval_Homing;
					HomingProjectilePool->GetObject(GetActorTransform());
				},
				Interval_Homing, true);
		}
		break;
	case APlayerWing::EProjectileType::Area:
		if (IsFireReady_Area())
		{			
			FireCoolTime_Area = Interval_Area;
			AreaProjectilePool->GetObject(GetActorTransform());
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
		TimerManager.ClearTimer(TimerHandle_Normal);	// 연사용 타이머 해제
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
	//test = true;
	static bool bFireLocationSelector = false;	// static 지	역변수로 선언하여 함수 호출시 계속 값을 유지하도록 함

	if (bFireLocationSelector)
	{
		bFireLocationSelector = false;
		return StaticMesh->GetSocketTransform(TEXT("FireSocket2"), ERelativeTransformSpace::RTS_World);	// 소켓의 트랜스폼 복사반환
	}
	else
	{
		bFireLocationSelector = true;
		return StaticMesh->GetSocketTransform(TEXT("FireSocket1"), ERelativeTransformSpace::RTS_World);
	}
}
