// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Item/EItemType.h"
#include "../Item/DropItemFactoryComponent.h"
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UDropItemFactoryComponent* DropItemFactory = nullptr;
};
