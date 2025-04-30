// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilder.h"
#include "Components/WidgetComponent.h"
#include "TowerDefence/Tower/UI/TowerBuilderWidget.h"

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
}

void ATowerBuilder::Test_BuildTower(int32 TowerIndex)
{
	UWorld* World = GetWorld();
	Tower = World->SpawnActor<ATower>(
		TowerDatas[TowerIndex]->TowerClass, GetActorLocation(), GetActorRotation());
}
