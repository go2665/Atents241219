// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItemToInventory(UItemDataAsset* InItemDataAsset)
{
	Inven.AddItem(InItemDataAsset);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Inven.Initialize();

	OnGoldChange.AddDynamic(this, &UInventoryComponent::TestGoldChange);	
}
