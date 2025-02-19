// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_Debris.h"
#include "Kismet/GameplayStatics.h"

APoolingEnemy_Debris::APoolingEnemy_Debris()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolingEnemy_Debris::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StaticMesh->AddRelativeRotation(FRotator(0.0f, RotatingSpeed * DeltaTime, 0.0f));
	StaticMesh->SetRelativeLocation(OrbitRadius * StaticMesh->GetForwardVector());
}

void APoolingEnemy_Debris::BeginPlay()
{
	Super::BeginPlay();
	GameModeCPP = Cast<AGameModeCPP>(UGameplayStatics::GetGameMode(GetWorld()));
}

void APoolingEnemy_Debris::OnActivate()
{
	Super::OnActivate();
	SetActorTickEnabled(true);
}

void APoolingEnemy_Debris::OnDeactivate()
{
	if (bIsInitialized)
	{
		SpawnSmallDebris();
	}

	SetActorTickEnabled(false);
	Super::OnDeactivate();	
}

void APoolingEnemy_Debris::SpawnSmallDebris()
{
	int32 DebrisCount = FMath::RandRange(RandomSmallDebrisCountMin, RandomSmallDebrisCountMax);
	float AngleStep = 360.0f / DebrisCount;

	for (int32 i = 0; i < DebrisCount; i++)
	{
		FVector SpawnLocation = StaticMesh->GetComponentLocation();
		FRotator SpawnRotation = FRotator(0.0f, i * AngleStep, 0.0f);
		
		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

		GameModeCPP->GetEnemy(EEnemyType::ET_SmallDebris, SpawnTransform);
	}
}
