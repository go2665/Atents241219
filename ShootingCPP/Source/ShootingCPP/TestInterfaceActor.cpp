// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInterfaceActor.h"

// Sets default values
ATestInterfaceActor::ATestInterfaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


}

// Called when the game starts or when spawned
void ATestInterfaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestInterfaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestInterfaceActor::TestNormalFunction()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TestNormal"));
}

void ATestInterfaceActor::TestNative_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("TestNative"));
}

