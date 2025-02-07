// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSpawnActor.h"
#include "TestInterfaceActor.h"

// Sets default values
ATestSpawnActor::ATestSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	ATestInterfaceActor* SpawnedActor = World->SpawnActor<ATestInterfaceActor>(
		ATestInterfaceActor::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
		
	ITestInterface::Execute_TestNative(SpawnedActor);	// SpawnedActor->TestNative();
	SpawnedActor->TestNormalFunction();
	
}

// Called every frame
void ATestSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

