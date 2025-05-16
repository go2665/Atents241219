// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilder.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Tower/UI/TowerBuilderWidget.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"

// Sets default values
ATowerBuilder::ATowerBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);

	TowerBuildWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerBuildWidget"));
	TowerBuildWidget->SetupAttachment(Root);
	TowerBuildWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TowerBuildWidget->SetDrawSize(FVector2D(600.0f, 600.0f));	// 위젯 크기 설정
}

bool ATowerBuilder::SetTowerOnce(ATower* InTower)
{
	bool bResult = false;	// 타워 설정 성공 여부
	if (!Tower)
	{
		Tower = InTower; 
		Tower->SetActorLocation(GetActorLocation());	// 타워 위치 설정
				
		Tower->OnTowerSell.AddLambda(
			[this](int32 _)
			{
				OnTowerBuildStateChange.ExecuteIfBound(false);
				Tower = nullptr;	// 타워가 팔렸을 때 빌더의 Tower를 nullptr로 초기화
			}
		);

		Tower->ActivateTower();
		OnTowerBuildStateChange.ExecuteIfBound(true);	// 타워가 설정되었음을 알림
		bResult = true;	// 타워 설정 성공
	}

	return bResult;
}

// Called when the game starts or when spawned
void ATowerBuilder::BeginPlay()
{
	Super::BeginPlay();
	
	// 타워 업그레이드 UI 위젯 초기화
	TowerBuilderWidgetInstance = Cast<UTowerBuilderWidget>(TowerBuildWidget->GetUserWidgetObject());
	if (TowerBuilderWidgetInstance)
	{
		TowerBuilderWidgetInstance->OnInitialize(&TowerDatas);
		//TowerBuilderWidgetInstance->OnTowerBuildRequest.BindLambda(
		//	[this](int32 InIndex)
		//	{
		//		//UE_LOG(LogTemp, Warning, TEXT("Tower Build Request! Index : %d"), InIndex);
		//		BuildTower(InIndex);
		//	}
		//);

		// 타워 빌드 요청 시 실행될 함수 바인딩
		TowerBuilderWidgetInstance->OnTowerBuildRequest.BindUObject(this, &ATowerBuilder::BuildTower);	
	}

	// 타워 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 열기)
	OnClicked.AddDynamic(this, &ATowerBuilder::OnBuilderClicked);

	// 타워 밖 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 닫기)
	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnMouseClickInput.AddUObject(this, &ATowerBuilder::OnScreenClicked);
	}
}

void ATowerBuilder::Test_BuildTower(int32 TowerIndex)
{
	BuildTower(TowerIndex);	
}

void ATowerBuilder::OnBuilderClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	//UE_LOG(LogTemp, Warning, TEXT("ATowerBuilder:OnBuilderClicked : Mouse Clicked!"));
	// 타워 클릭 시 업그레이드 UI 위젯 열기
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		if (TowerBuilderWidgetInstance && !Tower)
		{
			TowerBuilderWidgetInstance->Open();
			if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Clicked TowerBuilder!"), *this->GetActorNameOrLabel());
		}
	}
}

void ATowerBuilder::OnScreenClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("ATowerBuilder:OnScreenClicked : Mouse Clicked!"));
	if (TowerBuilderWidgetInstance)
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
			TowerBuilderWidgetInstance->Close();
			if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Builder Widget!"), *this->GetActorNameOrLabel());
		}
	}
	//else
	//{
	//	if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Not Close Builder Widget!"), *InClickedBuilder->GetActorNameOrLabel());
	//}
}

void ATowerBuilder::BuildTower(int32 InTowerIndex)
{
	if (!Tower)
	{
		UWorld* World = GetWorld();
		ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(World->GetAuthGameMode());
		
		if (GameMode->UseGold(TowerDatas[InTowerIndex]->TowerCost)) // 골드 사용 시도
		{
			ATower* NewTower = World->SpawnActor<ATower>(
				TowerDatas[InTowerIndex]->TowerClass, GetActorLocation(), GetActorRotation());
			NewTower->SetInitialTowerData(TowerDatas[InTowerIndex]);	// 타워 데이터 초기화
			NewTower->OnTowerSell.AddUObject(GameMode, &ATowerDefenceGameMode::AddGold);	// 타워가 팔렸을 때 골드 추가하도록 함수 연결

			SetTowerOnce(NewTower);		// 타워를 빌더에 설정
		}
		//else
		//{
		//	// 골드 부족 시 처리
		//	UE_LOG(LogTemp, Warning, TEXT("Not enough gold!"));
		//}
	}
}
