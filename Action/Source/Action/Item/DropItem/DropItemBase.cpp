// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "../ItemData/ItemDataAsset.h"

// Sets default values
ADropItemBase::ADropItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->SetCollisionProfileName(TEXT("DropItemProfile"));
	CollisionSphere->SetSimulatePhysics(true);				// 물리 시뮬레이션 시작
	CollisionSphere->SetNotifyRigidBodyCollision(true);		// 충돌 이벤트 발생	

	CollisionSphere->BodyInstance.bLockXRotation = true;	// 회전 제한
	CollisionSphere->BodyInstance.bLockYRotation = true;
	CollisionSphere->BodyInstance.bLockZRotation = true;
	CollisionSphere->BodyInstance.bLockRotation = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(CollisionSphere);
	ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	HighlightEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HighlightEffect"));
	HighlightEffect->SetupAttachment(ItemMesh);
	HighlightEffect->SetAutoActivate(false);
}

void ADropItemBase::InitializeItemDataAsset(UItemDataAsset* InItemDataAsset)
{	
	if (!bInitialized && InItemDataAsset) // 초기화 되지 않았고 InItemDataAsset가 null이 아닐때만 데이터 세팅
	{
		bInitialized = true;
		ItemDataAsset = InItemDataAsset;

		// 메시 설정
		ItemMesh->SetStaticMesh(ItemDataAsset->ItemMesh);			// 데이터 에셋에서 메시 가져오기

		// 머티리얼 설정
		if (ItemDataAsset->ItemMaterial)
		{
			ItemMesh->SetMaterial(0, ItemDataAsset->ItemMaterial);	// 데이터 에셋에서 머티리얼 가져오기
		}

		// 강조 이펙트 설정
		HighlightEffect->SetAsset(ItemDataAsset->HighlightEffect);	// 데이터 에셋에서 강조 이펙트 가져오기
		if (HighlightEffect)
		{
			HighlightEffect->Activate();	// 강조 이펙트 활성화
		}
	}	
}

void ADropItemBase::SetVelocity(const FVector& InVelocity)
{
	CollisionSphere->AddImpulse(InVelocity, NAME_None, true);	// 특정 방향으로 물리 힘을 가함
}

// Called when the game starts or when spawned
void ADropItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeItemDataAsset(ItemDataAsset);
	SetLifeSpan(LifeSpan);	// 일정 시간이 지나면 제거

	OnActorHit.AddDynamic(this, &ADropItemBase::OnDropItemHit);

	OnActorBeginOverlap.AddDynamic(this, &ADropItemBase::OnDropItemOverlap);
}

// Called every frame
void ADropItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivated)	// 바닥에 떨어졌을 때 회전하며 움직이기 시작
	{
		ItemMesh->AddRelativeRotation(FRotator(0.0f, 360.0f * DeltaTime, 0.0f));

		ElapsedTime += DeltaTime;
		//(0.5f * (FMath::Cos(ElapsedTime) - 1)) * 150.0f;
		ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f * (0.5f * (1 - FMath::Cos(ElapsedTime)))));
	}	

	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("ADropItemBase::Tick - %s"), *ItemDataAsset->ItemName.ToString()));
}

void ADropItemBase::OnDropItemHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag(TEXT("Item")))	// 아이템을 제외하고 충돌이 발생하면 바닥에 닿은 것으로 간주
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit"));
		CollisionSphere->SetSimulatePhysics(false);	// 물리 시뮬레이션 중지
		bIsActivated = true;						// 회전 시작	
	}
}

void ADropItemBase::OnDropItemOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bIsActivated && OtherActor->ActorHasTag(TEXT("Player")))	// 활성화 되어 있고 플레이어와 오버랩되면
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player Overlap"));

		// 아이템 획득 처리
		OnGetItem(OtherActor);	// 즉시 사용되는 아이템(ADropItem_Instance) or 인벤토리에 추가되는 아이템(ADropItem_Pickup)		
	}
}

