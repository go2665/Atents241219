// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilderActor.h"

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

