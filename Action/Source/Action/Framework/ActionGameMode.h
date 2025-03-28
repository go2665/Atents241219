// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Item/EItemType.h"
#include "../Item/DropItemFactoryComponent.h"
#include "SaveGame_Rank.h"
#include "ActionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AActionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AActionGameMode();

	UFUNCTION(BlueprintCallable, Category = "DropItem")
	inline class ADropItemBase* GetDropItem(
		EItemType InItemType,
		FVector InLocation = FVector::ZeroVector, FRotator InRotation = FRotator::ZeroRotator)
	{
		return DropItemFactory ? DropItemFactory->GetDropItem(InItemType, InLocation, InRotation) : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "DropItem")
	inline class UItemDataAsset* GetItemDataAsset(EItemType InItemType) const {
		return DropItemFactory ? DropItemFactory->GetItemDataAsset(InItemType) : nullptr;
	};

	// 게임이 클리어 되었을 때 호출
	UFUNCTION(BlueprintCallable, Category = "Clear")
	void OnGameClear();

	// 랭킹 데이터 추가
	UFUNCTION(BlueprintCallable, Category = "Rank")
	void AddRankData(int32 InGold);

	// 랭커 이름 설정(랭킹 데이터 인덱스(0~14), 랭커 이름)
	UFUNCTION(BlueprintCallable, Category = "Rank")
	void SetRankerName(int32 InRank, const FText& InName);

	const inline TArray<FRankData>& GetRankDataArray() const { return RankDataArray; }

	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestLoadRankData();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestSaveRankData();

protected:
	virtual void BeginPlay() override;

private:
	void LoadRankData();
	void SaveRankData() const;
	void InitializeDefaultRankData();
	void SortRankData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UDropItemFactoryComponent* DropItemFactory = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank")
	TArray<FRankData> RankDataArray;

private:
	// 유효한 랭킹 데이터 개수
	static const int32 RankDataCount = 15;

	// 랭킹 데이터 저장 슬롯 인덱스
	static const int32 SaveSlotindex = 0;

	// 메인 HUD
	UPROPERTY()
	class AMainHUD* MainHUD = nullptr;
};
