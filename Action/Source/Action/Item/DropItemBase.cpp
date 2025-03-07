// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "../Item/ItemDataAsset.h"

// Sets default values
ADropItemBase::ADropItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->SetCollisionProfileName(TEXT("DropItemProfile"));
	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->BodyInstance.bLockXRotation = true;
	CollisionSphere->BodyInstance.bLockYRotation = true;
	CollisionSphere->BodyInstance.bLockZRotation = true;
	CollisionSphere->BodyInstance.bLockRotation = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(CollisionSphere);
	ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	HighlightEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HighlightEffect"));
	HighlightEffect->SetupAttachment(CollisionSphere);
}

// Called when the game starts or when spawned
void ADropItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemDataAsset)
	{
		ItemMesh->SetStaticMesh(ItemDataAsset->ItemMesh);
	}

	OnActorHit.AddDynamic(this, &ADropItemBase::OnDropItemHit);
}

// Called every frame
void ADropItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartRotate)
	{
		ItemMesh->AddRelativeRotation(FRotator(0.0f, 360.0f * DeltaTime, 0.0f));

		ElapsedTime += DeltaTime;
		//(0.5f * (FMath::Cos(ElapsedTime) - 1)) * 150.0f;
		ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f * (0.5f * (1 - FMath::Cos(ElapsedTime)))));
	}	
}

void ADropItemBase::OnDropItemHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag(TEXT("Item")))
	{
		CollisionSphere->SetSimulatePhysics(false);
		bStartRotate = true;
	}
}

