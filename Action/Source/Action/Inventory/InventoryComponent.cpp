// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItemToInventory(UItemDataAsset* InItemDataAsset)
{
	return Inven.AddItem(InItemDataAsset);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Inven.Initialize();

	OnGoldChange.AddDynamic(this, &UInventoryComponent::TestGoldChange);	
}
