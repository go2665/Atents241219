// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Player/ActionPlayerCharacter.h"
#include "../Framework/ActionGameMode.h"
#include "../Item/EItemType.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::TestInventoryAddDefaultItems()
{
	// 인벤토리에 사과10개, 도끼 1개, 사과3개
	UWorld* World = GetWorld();
	AActionGameMode* GameMode = World->GetAuthGameMode<AActionGameMode>();

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
