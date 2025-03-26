// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameMode.h"
#include "MainHUD.h"

AActionGameMode::AActionGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	DropItemFactory = CreateDefaultSubobject<UDropItemFactoryComponent>(TEXT("DropItemFactoryComponent"));
}

void AActionGameMode::OnGameClear()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->DisableInput(PlayerController);	// 입력 비활성화
	PlayerController->SetShowMouseCursor(true);

	AMainHUD* MainHUD = PlayerController->GetHUD<AMainHUD>();
	MainHUD->GameClear();	// 게임 클리어 UI 표시
}

void AActionGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 랭킹 데이터 로드
	// 로드 실패시 초기화 함수로 데이터 세팅
	InitializeDefaultRankData();
}

void AActionGameMode::InitializeDefaultRankData()
{
	int32 Count = 16;
	RankDataArray.Empty();
	RankDataArray.Reserve(Count);

	char BaseChar = 'A';
	for (int32 i = 0; i < Count; ++i)
	{
		RankDataArray.Add(
			FRankData(
				FText::FromString(FString::Printf(TEXT("%c%c%c"), BaseChar + i, BaseChar + i, BaseChar + i)), 
				(Count - i - 1) * 100));		
	}
}
