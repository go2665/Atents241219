// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Tower/Cannon/Cannon.h"

// Sets default values
ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TowerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBodyMesh"));
	TowerBodyMesh->SetupAttachment(Root);
	TowerBodyMesh->SetCollisionProfileName(TEXT("Tower"));

	UpgradeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpgradeWidget"));
	UpgradeWidget->SetupAttachment(Root);
	UpgradeWidget->SetWidgetSpace(EWidgetSpace::Screen);	
	UpgradeWidget->SetDrawSize(FVector2D(200.0f, 600.0f));	// 위젯 크기 설정
}

void ATower::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!CannonInstance && CannonDatas.Num() > 0 && CannonDatas[0] != nullptr)
	{
		CannonInstance = GetWorld()->SpawnActor<ACannon>(
			CannonDatas[0]->CannonClass,
			FTransform::Identity);
		CannonInstance->AttachToComponent(
			TowerBodyMesh, 
			FAttachmentTransformRules::KeepRelativeTransform, TEXT("TowerRoof"));
	}
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	if (CannonInstance)
	{
		CannonInstance->OnInitialize(this); // 순서 기준
		//CannonInstance->OnInitialize(this, ACannon::EFireOrder::Distance); // 거리 기준
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] : CannonInstance is nullptr!"), *this->GetActorNameOrLabel());
	}
}

void ATower::TowerLevelUp()
{
	// Range, FireRate, TargetCount 등에 기본 값과 모디파이어를 곱한 값을 설정
}

void ATower::TowerSell()
{
}

void ATower::OnScreenClicked(AActor* InClickedTower)
{
}

void ATower::RefreshModifiers()
{
}

