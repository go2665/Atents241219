// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Item/ItemDataAsset.h"

void Inventory::AddItem(UItemDataAsset* InItemDataAsset)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, 
		FString::Printf(TEXT("AddItem : %s"), *InItemDataAsset->ItemName.ToString()));
}
