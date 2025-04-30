// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilder.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Tower/UI/TowerBuilderWidget.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"

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

// Called when the game starts or when spawned
void ATowerBuilder::BeginPlay()
{
	Super::BeginPlay();
	
	// 타워 업그레이드 UI 위젯 초기화
	TowerBuilderWidgetInstance = Cast<UTowerBuilderWidget>(TowerBuildWidget->GetUserWidgetObject());
	if (TowerBuilderWidgetInstance)
	{
		TowerBuilderWidgetInstance->OnInitialize(&TowerDatas);
		TowerBuilderWidgetInstance->OnTowerBuildRequest.BindLambda(
			[this](int32 InIndex)
			{
				UE_LOG(LogTemp, Warning, TEXT("Tower Build Request! Index : %d"), InIndex);
			}
		);
	}

	// 타워 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 열기)
	OnClicked.AddDynamic(this, &ATowerBuilder::OnBuilderClicked);

	// 타워 밖 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 닫기)
	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnMouseClickInput.AddDynamic(this, &ATowerBuilder::OnScreenClicked);
	}
}

void ATowerBuilder::Test_BuildTower(int32 TowerIndex)
{
	UWorld* World = GetWorld();
	Tower = World->SpawnActor<ATower>(
		TowerDatas[TowerIndex]->TowerClass, GetActorLocation(), GetActorRotation());
}

void ATowerBuilder::OnBuilderClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	// 타워 클릭 시 업그레이드 UI 위젯 열기
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		if (TowerBuilderWidgetInstance)
		{
			TowerBuilderWidgetInstance->Open();
			//if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Clicked TowerBuilder!"), *this->GetActorNameOrLabel());
		}
	}
}

void ATowerBuilder::OnScreenClicked(AActor* InClickedBuilder)
{
	if (TowerBuilderWidgetInstance && InClickedBuilder != this)
	{
		TowerBuilderWidgetInstance->Close();
		//if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Builder Widget!"), *this->GetActorNameOrLabel());
	}
	//else
	//{
	//	if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Not Close Builder Widget!"), *InClickedBuilder->GetActorNameOrLabel());
	//}
}
