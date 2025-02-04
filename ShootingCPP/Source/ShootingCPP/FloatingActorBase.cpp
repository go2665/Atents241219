// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActorBase.h"

AFloatingActorBase::AFloatingActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloatingStaticMesh"));
	StaticMesh->SetupAttachment(root);
}

// Called every frame
void AFloatingActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ElapsedTime += DeltaTime;
	//double time = GetWorld()->GetTimeSeconds();

	OnUpdateFloatMesh(DeltaTime);	
}

void AFloatingActorBase::OnConstruction(const FTransform& Transform)
{
	BaseHeight = StaticMesh->GetComponentLocation().Z;
}

void AFloatingActorBase::OnUpdateFloatMesh(float DeltaTime)
{
	float cosValue = FMath::Cos(ElapsedTime);	// 1 -> -1 -> 1
	cosValue += 1;								// 2 -> 0 -> 2
	cosValue *= 0.5f;							// 1 -> 0 -> 1
	cosValue = 1 - cosValue;					// 0 -> 1 -> 0

	StaticMesh->SetRelativeLocation(FVector(0, 0, BaseHeight + cosValue * MoveHeight));
	StaticMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));
}

