// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItemToInventory(UItemDataAsset* InItemDataAsset)
{
	return Inven.AddItem(InItemDataAsset);	// 인벤토리에 있는 아이템 추가 함수 실행
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
