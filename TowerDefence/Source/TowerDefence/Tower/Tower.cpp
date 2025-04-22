// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Tower/Cannon.h"
#include "TowerDefence/Tower/UI/TowerUpgradeWidget.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Enemy/EnemyBase.h"

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

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	verify(CannonDatas.Num() > 0);

	// 타워 데이터 초기화
	UpdateData();

	// 대포 인스턴스가 없으면 생성하고 초기화
	if (!CannonInstance)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CannonInstance = GetWorld()->SpawnActor<ACannon>(
			CannonDatas[0]->CannonClass,
			FTransform::Identity,
			SpawnParams);
		CannonInstance->AttachToComponent(
			TowerBodyMesh,
			FAttachmentTransformRules::KeepRelativeTransform, TEXT("TowerRoof"));

		CannonInstance->OnInitialize(this, GetCannonData()->FireOrder);		// 발사 순서 기준
		CannonInstance->OnCannonFire.AddDynamic(this, &ATower::TowerFire);	// 발사 시 타워에 신호 전달
	}
	
	// 타워 업그레이드 UI 위젯 초기화
	UpgradeWidgetInstance = Cast<UTowerUpgradeWidget>(UpgradeWidget->GetUserWidgetObject());
	if (UpgradeWidgetInstance)
	{
		UpgradeWidgetInstance->OnUpgradeClicked.BindUFunction(this, FName("TowerLevelUp"));
		UpgradeWidgetInstance->OnSellClicked.BindUFunction(this, FName("TowerSell"));
	}

	// 타워 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 열기)
	OnClicked.AddDynamic(this, &ATower::OnTowerClicked);

	// 타워 밖 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 닫기)
	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnMouseClickInput.AddDynamic(this, &ATower::OnScreenClicked);
	}

	// 판매 비용 초기화
	SellCost = TowerCost * 0.5f;	// 판매 비용은 (설치 비용 + 업그레이드 비용)의 절반
}

void ATower::TowerLevelUp()
{
	if (TowerLevel < MaxTowerLevel)
	{
		float UpgradeCost = GetCannonData()->UpgradeCost;
		ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode->UseGold(UpgradeCost))	// 업그레이드 비용 차감 시도
		{
			TowerLevel++;					// 타워 레벨 증가
			SellCost += UpgradeCost * 0.5f; // 판매 비용 증가
			
			UpdateData();	// 타워 데이터 갱신
		}
	}
}

void ATower::TowerSell()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Sell!"), *this->GetActorNameOrLabel());
	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->AddGold(SellCost);	// 판매 비용 추가
	if (CannonInstance)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Sell : Cannon Destroyed!"), *CannonInstance->GetActorNameOrLabel());
		CannonInstance->Destroy();	// 대포 인스턴스 삭제
		CannonInstance = nullptr;
	}
	Destroy();	// 타워 삭제	
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Sell : Tower Destroyed!"), *this->GetActorNameOrLabel());
}

void ATower::TowerFire(const TArray<AEnemyBase*>& InTargetEnemies)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Fire!"), *this->GetActorNameOrLabel());
	//Test_PrintFireTargetList(InTargetEnemies);	// 공격하는 적 목록 출력


}

void ATower::OnTowerClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	// 타워 클릭 시 업그레이드 UI 위젯 열기
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		if (UpgradeWidgetInstance)
		{
			UpgradeWidgetInstance->OpenUpgradeWidget(GetCannonData()->UpgradeCost);
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : Clicked Tower!"), *this->GetActorNameOrLabel());
		}
	}
}

void ATower::OnScreenClicked(AActor* InClickedTower)
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

void ATower::UpdateModifiers()
{
	// 버프나 디버프 추가/삭제가 있을 때마다 실행
}

void ATower::UpdateData()
{
	UpdateModifiers();

	// Range, FireRate, TargetCount 등에 기본 값과 모디파이어를 곱한 값을 설정
	Range = GetCannonData()->Range;			// 모디파이어 곱하기 적용할 것
	FireRate = GetCannonData()->FireRate;
	TargetCount = GetCannonData()->TargetCount;
}

void ATower::Test_PrintFireTargetList(const TArray<AEnemyBase*>& InTargetEnemies)
{
	FString EnemyList;
	for (AEnemyBase* Enemy : InTargetEnemies)
	{
		if (Enemy)
		{
			EnemyList += Enemy->GetName() + TEXT(", ");
		}
	}
	EnemyList += TEXT("Total: ") + FString::FromInt(InTargetEnemies.Num());

	UE_LOG(LogTemp, Warning, TEXT("Enemies in range: [ %s ]"), *EnemyList);
}

