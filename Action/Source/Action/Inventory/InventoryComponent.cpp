// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Player/ActionPlayerCharacter.h"
#include "../Framework/ActionGameMode.h"
#include "../Item/EItemType.h"
#include "../Item/FDropItemDataTableRow.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::TestInventoryAddDefaultItems(UDataTable* TestTable)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AActionGameMode* GameMode = World->GetAuthGameMode<AActionGameMode>();

		if (TestTable)
		{
			TMap<FName, uint8*> RowMap = TestTable->GetRowMap();
			for (auto& Elem : RowMap)
			{
				FDropItemDataTableRow* Row = (FDropItemDataTableRow*)Elem.Value;

				for (int i = 0; i < Row->Count; ++i)	// Count만큼 아이템 생성
				{
					UItemDataAsset* Data = GameMode->GetItemDataAsset(Row->ItemType);
					AddItemToInventory(Data);
				}
			}
		}
		else
		{
			// 데이터 테이블이 없으면 인벤토리에 사과10개, 도끼 1개, 사과3개
			UItemDataAsset* Data = GameMode->GetItemDataAsset(EItemType::Apple);
			for (int i = 0; i < 10; i++)
			{
				AddItemToInventory(Data);
			}

			Data = GameMode->GetItemDataAsset(EItemType::Axe);
			AddItemToInventory(Data);

			Data = GameMode->GetItemDataAsset(EItemType::Apple);
			for (int i = 0; i < 3; i++)
			{
				AddItemToInventory(Data);
			}
		}
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(PlayerController->GetPawn());
	Inven.Initialize(Player);	// 인벤토리 초기화

	OnGoldChange.AddDynamic(this, &UInventoryComponent::TestGoldChange);	// 골드 변화 델리게이트에 함수 추가
}
