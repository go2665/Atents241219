// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_SmallDebris.h"

APoolingEnemy_SmallDebris::APoolingEnemy_SmallDebris()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolingEnemy_SmallDebris::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat Quat = FQuat(RotateAxis, FMath::DegreesToRadians(RotatingSpeed * DeltaTime));
	StaticMesh->AddLocalRotation(Quat);
}

void APoolingEnemy_SmallDebris::OnActivate()
{
	Super::OnActivate();
	SetActorTickEnabled(true);
	RotateAxis = FMath::VRand();

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		RandomDirectionTimer,
		this, &APoolingEnemy_SmallDebris::SetRandomDirection, 
		FMath::FRandRange(RandomDirectionIntervalMin, RandomDirectionIntervalMax), 
		false);
}

void APoolingEnemy_SmallDebris::OnDeactivate()
{
	Super::OnDeactivate();
	SetActorTickEnabled(false);
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearTimer(RandomDirectionTimer);
	RandomDirectionTimer.Invalidate();
}

void APoolingEnemy_SmallDebris::SetRandomDirection()
{	
	FVector RandomDirection(
		FMath::FRandRange(-1.0f, 1.0f), 
		FMath::FRandRange(-1.0f, 1.0f), 
		0);
	RandomDirection.Normalize();

	ProjectileMovement->Velocity = MoveSpeed * RandomDirection;

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		RandomDirectionTimer,
		this, &APoolingEnemy_SmallDebris::SetRandomDirection,
		FMath::FRandRange(RandomDirectionIntervalMin, RandomDirectionIntervalMax),
		false);
}
