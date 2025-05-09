// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBaseActor.h"
#include "Components/ChildActorComponent.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Gun/GunBaseActor.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/UI/DEPRECATED_TowerUpgradeWidget.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Effect/Buff/TowerBuffComponent.h"

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
	UpgradeWidget->SetWidgetClass(U_DEPRECATED_TowerUpgradeWidget::StaticClass());	// 위젯 클래스 설정(블루프린트로 만든 것을 새로 넣어주어야 함)
	UpgradeWidget->SetDrawSize(FVector2D(200.0f, 600.0f));	// 위젯 크기 설정

	BuffComponent = CreateDefaultSubobject<UTowerBuffComponent>(TEXT("BuffComponent"));	
}

void ATowerBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (GunActor && GunActor->GetChildActor() == nullptr)
	{
		GunActor->SetChildActorClass(GunClass);		

		//if (GunClass)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("[%s] : GunClass is %s"),
		//		*GetActorNameOrLabel(), *GunClass->GetName());
		//}
	}
}

// Called when the game starts or when spawned
void ATowerBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GunClass && !Gun)
	{
		Gun = Cast<AGunBaseActor>(GunActor->GetChildActor());
		Gun->OnInitialize(this);	// 총기 소유자 설정

		//UE_LOG(LogTemp, Warning, TEXT("[%s] : Gun is %s"),
		//	*GetActorNameOrLabel(), *Gun->GetActorNameOrLabel());
	}

	UpgradeWidgetInstance = Cast<U_DEPRECATED_TowerUpgradeWidget>(UpgradeWidget->GetUserWidgetObject());
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
		PlayerController->OnMouseClickInput.AddUObject(this, &ATowerBaseActor::OnCancelClicked);
	}

	SellCost = TowerCost * 0.5f;	// 판매 비용은 (설치 비용 + 업그레이드 비용)의 절반
}

void ATowerBaseActor::LevelUp()
{
	if (GunLevel < MaxGunLevel)
	{
		GunLevel++;
		ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->UseGold(GetCurrentUpgradeCost()); // 업그레이드 비용 차감

		SellCost += Gun->GetCurrentUpgradeCost() * 0.5f; // 판매 비용 증가

		if (Gun)
		{
			Gun->SetGunLevel(GunLevel); // 총기 레벨 설정
		}
	}
}

void ATowerBaseActor::Sell()
{
	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->AddGold(SellCost); // 판매 비용 추가

	Destroy();
}

void ATowerBaseActor::AddBuff(UTowerBuffDataAsset* BuffData)
{
	if (GetBuffComponent())
	{
		GetBuffComponent()->OnAddedBuff(BuffData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] : BuffComponent is nullptr!"), *this->GetActorNameOrLabel());
	}
}

void ATowerBaseActor::RefreshBuffModifiers()
{
	Gun->RefreshBuffModifiers();	// 총기 모디파이어 재적용
}

float ATowerBaseActor::GetBuffModifierValue(ETowerBuffModifier Type)
{
	if (GetBuffComponent())
	{
		return GetBuffComponent()->GetBuffModifierValue(Type);
	}
	return 1.0f;
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

void ATowerBaseActor::OnCancelClicked()
{
	if (UpgradeWidgetInstance)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		AActor* HitActor = nullptr;	// 클릭한 액터를 저장할 변수
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult))	// ECC_GameTraceChannel2(Tower)로 트레이스
		{
			HitActor = HitResult.GetActor();
		}
		//if (HitActor)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Clicked on: %s"), *HitActor->GetActorNameOrLabel());
		//}

		if (HitActor != this)
		{
			UpgradeWidgetInstance->CloseUpgradeWidget();
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Upgrade Widget!"), *this->GetActorNameOrLabel());
		}
	}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[%s] : Not Close Upgrade Widget!"), *InClickedTower->GetActorNameOrLabel());
	//}
}


