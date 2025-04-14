// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBaseActor.h"
#include "Components/ChildActorComponent.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Gun/GunBaseActor.h"
#include "TowerDefence/Tower/UI/TowerUpgradeWidget.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"

// Sets default values
ATowerBaseActor::ATowerBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TowerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBodyMesh"));
	TowerBodyMesh->SetupAttachment(Root);
	//TowerBodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TowerBodyMesh->SetCollisionProfileName(TEXT("Tower"));

	GunActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("GunActor"));
	GunActor->SetupAttachment(TowerBodyMesh, TEXT("TowerRoof"));

	UpgradeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpgradeWidget"));
	UpgradeWidget->SetupAttachment(Root);
	UpgradeWidget->SetWidgetSpace(EWidgetSpace::Screen);		
}

void ATowerBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (GunActor && GunActor->GetChildActor() == nullptr)
	{
		GunActor->SetChildActorClass(GunClass);
		Gun = Cast<AGunBaseActor>(GunActor->GetChildActor());
	}
}

// Called when the game starts or when spawned
void ATowerBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	//if (GunClass)
	//{
	//	GunActor->SetChildActorClass(GunClass);
	//	Gun = Cast<AGunBaseActor>(GunActor->GetChildActor());
	//}

	UpgradeWidgetInstance = Cast<UTowerUpgradeWidget>(UpgradeWidget->GetUserWidgetObject());
	if (UpgradeWidgetInstance)
	{
		UpgradeWidgetInstance->UpgradeWidgetInitialize(this);	// 위젯에 타워 설정
		//UpgradeWidgetInstance->Test_PrintTower();
	}

	OnClicked.AddDynamic(this, &ATowerBaseActor::OnTowerClicked);

	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController = 
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnMouseClickInput.AddDynamic(this, &ATowerBaseActor::OnCancelClicked);
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

void ATowerBaseActor::OnTowerClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		if (UpgradeWidgetInstance)
		{
			UpgradeWidgetInstance->OpenUpgradeWidget();
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : Clicked Tower!"), *this->GetActorNameOrLabel());
		}
	}
}

void ATowerBaseActor::OnCancelClicked(AActor* InClickedTower)
{
	if (UpgradeWidgetInstance && InClickedTower != this)
	{
		UpgradeWidgetInstance->CloseUpgradeWidget();
		//UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Upgrade Widget!"), *this->GetActorNameOrLabel());
	}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[%s] : Not Close Upgrade Widget!"), *InClickedTower->GetActorNameOrLabel());
	//}
}


