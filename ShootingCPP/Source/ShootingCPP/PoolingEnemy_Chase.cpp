// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_Chase.h"
#include "PlayerWingUsePool.h"

APoolingEnemy_Chase::APoolingEnemy_Chase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolingEnemy_Chase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPawn)
	{
		BehaviorPhase1(DeltaTime);
	}
	else
	{
		BehaviorPhase2(DeltaTime);
	}
}

void APoolingEnemy_Chase::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	APlayerWingUsePool* Wing = Cast<APlayerWingUsePool>(PlayerController->GetPawn());
	Wing->OnDie.AddDynamic(this, &APoolingEnemy_Chase::TargetLost);
}

void APoolingEnemy_Chase::OnActivate()
{
	Super::OnActivate();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	TargetPawn = PlayerController->GetPawn();	

	FTimerManager& TimerManager = World->GetTimerManager();
	FTimerHandle TempHandle;
	TimerManager.SetTimer(
		TempHandle,
		[this]() { TargetPawn = nullptr; },
		Phase1Duration, false);
}

void APoolingEnemy_Chase::OnDeactivate()
{
	Super::OnDeactivate();

	TargetPawn = nullptr;
}

void APoolingEnemy_Chase::BehaviorPhase1(float DeltaTime)
{
	FRotator Current = GetActorRotation();
	FVector Direction = TargetPawn->GetActorLocation() - GetActorLocation();
	Direction.Z = 0;
	FRotator Target = FRotationMatrix::MakeFromX(Direction).Rotator();

	FRotator NewRotator = FMath::RInterpConstantTo(Current, Target, DeltaTime, RotateSpeed);
	SetActorRotation(NewRotator);

	ProjectileMovement->Velocity = MoveSpeed * GetActorForwardVector();
}

void APoolingEnemy_Chase::BehaviorPhase2(float DeltaTime)
{
	// 앞쪽으로 가속
	// 최대 속도까지 가속
}
