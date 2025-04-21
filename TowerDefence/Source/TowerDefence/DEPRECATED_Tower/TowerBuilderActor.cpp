// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilderActor.h"
#include "TowerBaseActor.h"

// Sets default values
ATowerBuilderActor::ATowerBuilderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ATowerBuilderActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATowerBuilderActor::Test_BuildTower(int32 TowerIndex)
{
	UWorld* World = GetWorld();
	Tower = World->SpawnActor<ATowerBaseActor>(
		TowerClasses[TowerIndex], GetActorLocation(), GetActorRotation());
}

