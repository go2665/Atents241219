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
	CollisionSphere->SetSimulatePhysics(true);				// 물리 시뮬레이션 시작
	CollisionSphere->BodyInstance.bLockXRotation = true;	// 회전 제한
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
		ItemMesh->SetStaticMesh(ItemDataAsset->ItemMesh);	// 데이터 에셋에서 메시 가져오기
	}

	OnActorHit.AddDynamic(this, &ADropItemBase::OnDropItemHit);
}

// Called every frame
void ADropItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartRotate)	// 바닥에 떨어졌을 때 회전하며 움직이기 시작
	{
		ItemMesh->AddRelativeRotation(FRotator(0.0f, 360.0f * DeltaTime, 0.0f));

		ElapsedTime += DeltaTime;
		//(0.5f * (FMath::Cos(ElapsedTime) - 1)) * 150.0f;
		ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f * (0.5f * (1 - FMath::Cos(ElapsedTime)))));
	}	
}

void ADropItemBase::OnDropItemHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag(TEXT("Item")))	// 아이템을 제외하고 충돌이 발생하면 바닥에 닿은 것으로 간주
	{
		CollisionSphere->SetSimulatePhysics(false);	// 물리 시뮬레이션 중지
		bStartRotate = true;						// 회전 시작	
	}
}

