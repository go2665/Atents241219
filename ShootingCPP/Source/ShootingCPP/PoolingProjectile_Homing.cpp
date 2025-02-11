// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingProjectile_Homing.h"

APoolingProjectile_Homing::APoolingProjectile_Homing()
{
	HomingRadius = CreateDefaultSubobject<USphereComponent>(TEXT("HomingRadius"));
	HomingRadius->SetupAttachment(RootComponent);
	HomingRadius->SetSphereRadius(500);

	Movement->HomingAccelerationMagnitude = 1000.0f;	// 호밍 가속도
}

void APoolingProjectile_Homing::BeginPlay()
{
	Super::BeginPlay();

	// 호밍 반경에 다른 액터가 겹치면 호출할 함수 바인딩
	HomingRadius->OnComponentBeginOverlap.AddDynamic(
		this, &APoolingProjectile_Homing::OnHomingBeginOverlap);
}

void APoolingProjectile_Homing::OnDeactivate()
{
	Movement->bIsHomingProjectile = false;		// 호밍 여부	비활성화
	Movement->HomingTargetComponent = nullptr;	// 호밍 타겟 제거
	Super::OnDeactivate();
}

void APoolingProjectile_Homing::OnHomingBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Movement->HomingTargetComponent == nullptr		// 호밍 대상이 없을 때만 처리
		&& OtherActor->ActorHasTag("Enemy"))			// 적일 때만 처리
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, 
		//	FString::Printf(TEXT("Homing Begin Overlap : %s"), *OtherActor->GetActorLabel()));

		Movement->HomingTargetComponent = OtherComp;	// 호밍 대상 설정
		Movement->bIsHomingProjectile = true;			// 호밍 여부 활성화

		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		//TimerManager.SetTimer(	// 호밍 리셋 타이머 설정(맴버	함수 사용)
		//	HomingResetTimerHandle,
		//	this, &APoolingProjectile_Homing::HomingReset,
		//	HomingDuration, false);
		TimerManager.SetTimer(		// 호밍 리셋 타이머 설정(람다식 사용)
			HomingResetTimerHandle,
			[this]() { Movement->bIsHomingProjectile = false; },	// 호밍 여부	비활성화
			HomingDuration, false);	
	}
}

//void APoolingProjectile_Homing::HomingReset()
//{
//	Movement->bIsHomingProjectile = false;		// 호밍 여부	비활성화
//}
