// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemyStraight.h"

APoolingEnemyStraight::APoolingEnemyStraight()
{
	PrimaryActorTick.bCanEverTick = true;
	CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoreMesh"));
	CoreMesh->SetupAttachment(RootComponent);
	CoreMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CoreMesh->SetRelativeScale3D(FVector(0.5f));
}
