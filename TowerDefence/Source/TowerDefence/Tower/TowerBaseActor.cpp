// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBaseActor.h"
#include "Components/ChildActorComponent.h"
#include "TowerDefence/Gun/GunBaseActor.h"

// Sets default values
ATowerBaseActor::ATowerBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TowerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBodyMesh"));
	TowerBodyMesh->SetupAttachment(Root);
	TowerBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GunActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("GunActor"));
	GunActor->SetupAttachment(TowerBodyMesh, TEXT("TowerRoof"));
}

// Called when the game starts or when spawned
void ATowerBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GunClass)
	{
		GunActor->SetChildActorClass(GunClass);
		Gun = Cast<AGunBaseActor>(GunActor->GetChildActor());
	}
}

void ATowerBaseActor::LevelUp()
{
	if (GunLevel < MaxGunLevel)
	{
		GunLevel++;

		if (Gun)
		{
			Gun->SetGunLevel(GunLevel); // 총기 레벨 설정
		}
	}
}


