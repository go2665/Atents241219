// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "SpawnerBaseComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SpawnAreaBase = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Base"));
	SpawnAreaBase->SetupAttachment(Root);
	UArrowComponent* ArrowBase = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Base"));
	ArrowBase->SetupAttachment(SpawnAreaBase);
	ArrowBase->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerBase = CreateDefaultSubobject<USpawnerBaseComponent>(TEXT("Spawner_Base"));
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnerBase->SetSpawnArea(SpawnAreaBase);
}