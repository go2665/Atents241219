// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "PoolingObjectBase.h"
#include "Components/BoxComponent.h"

// Sets default values
AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BorderTop = CreateDefaultSubobject<UBoxComponent>(TEXT("BorderTop"));
	BorderTop->SetupAttachment(RootComponent);
	BorderTop->SetBoxExtent(FVector(1000.0f, 4000.0f, 1000.0f));
	BorderTop->SetRelativeLocation(FVector(3000.0f, 0.0f, 0.0f));

	BorderBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("BorderBottom"));
	BorderBottom->SetupAttachment(RootComponent);
	BorderBottom->SetBoxExtent(FVector(1000.0f, 4000.0f, 1000.0f));
	BorderBottom->SetRelativeLocation(FVector(-3000.0f, 0.0f, 0.0f));

	BorderLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("BorderLeft"));
	BorderLeft->SetupAttachment(RootComponent);
	BorderLeft->SetBoxExtent(FVector(2000.0f, 1000.0f, 1000.0f));
	BorderLeft->SetRelativeLocation(FVector(0.0f, -3000.0f, 0.0f));

	BorderRight = CreateDefaultSubobject<UBoxComponent>(TEXT("BorderRight"));
	BorderRight->SetupAttachment(RootComponent);
	BorderRight->SetBoxExtent(FVector(2000.0f, 1000.0f, 1000.0f));
	BorderRight->SetRelativeLocation(FVector(0.0f, 3000.0f, 0.0f));
	
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AKillZone::OnKillZoneOverlapBegin);
}

void AKillZone::OnKillZoneOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	APoolingObjectBase* PoolingObject = Cast<APoolingObjectBase>(OtherActor);
	if (PoolingObject)
	{
		PoolingObject->Deactivate();	// 풀링 오브젝트는 비활성화
	}
	else
	{
		OtherActor->Destroy();			// 풀링 오브젝트가 아니면 파괴	
	}
}
