// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameMode.h"
#include "MainHUD.h"
#include "Action/Player/ActionPlayerState.h"
#include "Action/Framework/SaveGame_Rank.h"
#include "Kismet/GameplayStatics.h"

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

	SaveRankData();	// 랭킹 데이터 저장
}

void AActionGameMode::AddRankData(int32 InGold)
{
	int32 NewRank = RankDataCount;	// 마지막 등수로 초기화
	for (int i = 0; i < RankDataCount; i++)
	{
		if (RankDataArray[i].Gold < InGold)	// 15등 안에 들어갔는지 확인
		{
			NewRank = i;			// 15등 안에 들어갔다면 NewRank에 등수 저장
			break;
		}
	}

	// SortRankData 함수를 사용할 경우 아래 3줄 주석 모두 풀것
	//RankDataArray[RankDataCount].Gold = InGold;
	//RankDataArray[RankDataCount].RankName = FText::FromString(TEXT("New Ranker"));
	//SortRankData();	

	// 랭킹 데이터 갱신(15등 안에 들어갔을 때만 처리)
	if (NewRank < RankDataCount)
	{
		for (int i = RankDataCount - 1; i > NewRank; --i)	// 14 -> NewRank+1까지 순회
		{
			RankDataArray[i] = RankDataArray[i - 1];
		}
		RankDataArray[NewRank].Gold = InGold;
		RankDataArray[NewRank].RankName = FText::FromString(TEXT("New Ranker"));
	}

	if (MainHUD)
	{
		MainHUD->RefreshRankList();	// UI 갱신

		// 15등 안에 들어갔을 때의 처리 필요
		if (NewRank < RankDataCount)
		{
			MainHUD->OnNewRanker(NewRank);
		}
	}
}

void AActionGameMode::SetRankerName(int32 InRank, const FText& InName)
{
	RankDataArray[InRank].RankName = InName;	// 입력 받은 결과를 랭킹 데이터에 반영

	SaveRankData();	// 랭킹 데이터 저장

	if (MainHUD)
	{
		MainHUD->RefreshRankList();	// UI 갱신
	}
}

void AActionGameMode::TestLoadRankData()
{
	LoadRankData();
}

void AActionGameMode::TestSaveRankData()
{
	SaveRankData();
}

void AActionGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 랭킹 데이터 로드
	LoadRankData();	
}

void AActionGameMode::LoadRankData()
{
	USaveGame_Rank* LoadedGame = Cast<USaveGame_Rank>(
		UGameplayStatics::LoadGameFromSlot(TEXT("RankingSaveSlot"), SaveSlotindex));
	
	if (LoadedGame)
	{
		RankDataArray = LoadedGame->RankDataArray;
		if (RankDataArray.IsEmpty())
		{
			InitializeDefaultRankData();	
		}
	}
	else
	{
		InitializeDefaultRankData();	// 로드 실패시 초기화 함수로 데이터 세팅
	}

	if (MainHUD)
	{
		MainHUD->RefreshRankList();		// UI 갱신
	}
}

void AActionGameMode::SaveRankData() const
{
	USaveGame_Rank* SaveGameInstance = Cast<USaveGame_Rank>(
		UGameplayStatics::CreateSaveGameObject(USaveGame_Rank::StaticClass()));

	SaveGameInstance->RankDataArray = RankDataArray;

	UGameplayStatics::SaveGameToSlot(
		SaveGameInstance, TEXT("RankingSaveSlot"), SaveSlotindex);
}

void AActionGameMode::InitializeDefaultRankData()
{
	int32 Count = RankDataCount + 1;	// 마지막 자리는 새로 추가되는 데이터를 위한 자리(+1은 Sort를 사용할 때만 유용)
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
