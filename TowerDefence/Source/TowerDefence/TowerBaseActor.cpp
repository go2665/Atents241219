// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBaseActor.h"
#include "Components/ChildActorComponent.h"

// Sets default values
ATowerBaseActor::ATowerBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATowerBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

