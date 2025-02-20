// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Components/ArrowComponent.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	AddSpawnerBaseComponent();
	AddSpawnerAreaComponent();
}

void ASpawner::AddSpawnerBaseComponent()
{
	SpawnAreaBase = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Base"));
	SpawnAreaBase->SetupAttachment(RootComponent);

	UArrowComponent* ArrowBase = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Base"));
	ArrowBase->SetupAttachment(SpawnAreaBase);
	ArrowBase->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerBase = CreateDefaultSubobject<USpawnerBaseComponent>(TEXT("Spawner_Base"));
	SpawnerBase->SetSpawnArea(SpawnAreaBase);
}

void ASpawner::AddSpawnerAreaComponent()
{
	SpawnAreaArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Area"));
	SpawnAreaArea->SetupAttachment(RootComponent);

	UArrowComponent* ArrowArea = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Area"));
	ArrowArea->SetupAttachment(SpawnAreaArea);
	ArrowArea->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerArea = CreateDefaultSubobject<USpawnerAreaComponent>(TEXT("Spawner_Area"));
	SpawnerArea->SetSpawnArea(SpawnAreaArea);
}
