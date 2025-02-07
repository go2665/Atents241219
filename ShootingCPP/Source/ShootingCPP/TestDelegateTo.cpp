// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDelegateTo.h"
#include "TestDelegateFrom.h"

// Sets default values
ATestDelegateTo::ATestDelegateTo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestDelegateTo::BeginPlay()
{
	Super::BeginPlay();
	ATestDelegateFrom* Spawned = GetWorld()->SpawnActor<ATestDelegateFrom>(ATestDelegateFrom::StaticClass());
	Spawned->OnFinish.AddUObject(this, &ATestDelegateTo::Test1);
	Spawned->TestRunDelegate();
}

// Called every frame
void ATestDelegateTo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestDelegateTo::Test1()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Test1"));
}

