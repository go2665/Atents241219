// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor2.h"

// Sets default values
ATestActor2::ATestActor2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

