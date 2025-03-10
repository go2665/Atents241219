// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_Pickup.h"
#include "ItemDataAsset.h"

void ADropItem_Pickup::OnGetItem()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,
		FString::Printf(TEXT("ADropItem_Pickup::OnGetItem - %s"), *ItemDataAsset->ItemName.ToString()));
}
