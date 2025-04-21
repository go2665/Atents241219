// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower/Cannon/CannonBase.h"

// Sets default values
ACannonBase::ACannonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACannonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

