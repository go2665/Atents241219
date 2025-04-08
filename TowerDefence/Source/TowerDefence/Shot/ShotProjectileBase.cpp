// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotProjectileBase.h"

// Sets default values
AShotProjectileBase::AShotProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AShotProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}
