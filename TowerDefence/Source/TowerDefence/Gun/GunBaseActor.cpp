// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBaseActor.h"

// Sets default values
AGunBaseActor::AGunBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);
	GunMesh->SetRelativeScale3D(FVector(ScaleFactor));
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

// Called when the game starts or when spawned
void AGunBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

