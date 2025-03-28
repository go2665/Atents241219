// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameMode.h"
#include "MainHUD.h"
#include "Action/Player/ActionPlayerState.h"

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

	MainHUD = PlayerController->GetHUD<AMainHUD>();
	MainHUD->GameClear();	// 게임 클리어 UI 표시

	AActionPlayerState* PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();	
	if (PlayerState)
	{
		AddRankData(PlayerState->GetGold());
	}
}

void AActionGameMode::AddRankData(int32 InGold)
{
	int32 NewRank = RankDataCount;
	for (int i = 0; i < RankDataCount; i++)
	{
		if (RankDataArray[i].Gold < InGold)
		{
			NewRank = i;
			break;
		}
	}

	RankDataArray[RankDataCount].Gold = InGold;
	RankDataArray[RankDataCount].RankName = FText::FromString(TEXT("New Ranker"));
	SortRankData();	

	if (MainHUD)
	{
		MainHUD->RefreshRankList();

		// 15등 안에 들어갔을 때의 처리 필요
		if (NewRank < RankDataCount)
		{
			MainHUD->OnNewRanker(NewRank);
		}
	}
}

void AActionGameMode::SetRankerName(int32 InRank, const FText& InName)
{
	RankDataArray[InRank].RankName = InName;

	if (MainHUD)
	{
		MainHUD->RefreshRankList();
	}
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
	int32 Count = RankDataCount + 1;	// 마지막 자리는 새로 추가되는 데이터를 위한 자리
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

void AActionGameMode::SortRankData()
{
	// 골드 기준으로 내림차순 정렬
	RankDataArray.Sort([](const FRankData& A, const FRankData& B) {
		return A.Gold > B.Gold;
		});
}
