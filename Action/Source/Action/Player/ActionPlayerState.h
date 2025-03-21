// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Inventory/InventoryComponent.h"
#include "ActionPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AActionPlayerState : public APlayerState
{
	GENERATED_BODY()	

public:
	AActionPlayerState();

protected:
	virtual void BeginPlay() override;

public:
	inline int32 GetGold() const { return InventoryComponent ? InventoryComponent->GetGold() : 0; }
	inline void AddGold(int32 Amount) { if (InventoryComponent) InventoryComponent->AddGold(Amount); }
	inline bool AddItemToInventory(class UItemDataAsset* InItemDataAsset) { 
		if (InventoryComponent)
			return InventoryComponent->AddItem(InItemDataAsset);
		else
			return false;
	}
	inline FOnPlayerGoldChange& GetOnGoldChange() { return InventoryComponent->OnGoldChange; }
	inline void MoveItemFromInventory(EInvenSlotType InFromSlotType, EInvenSlotType InToSlotType) {
		if (InventoryComponent) InventoryComponent->MoveItem(InFromSlotType, InToSlotType);
	}
	inline void UseItemFromInventory(EInvenSlotType InSlotType) {
		if (InventoryComponent) InventoryComponent->UseItem(InSlotType);
	}
	inline void EquipItemFromInventory(EInvenSlotType InSlotType) {
		if (InventoryComponent) InventoryComponent->EquipItem(InSlotType);
	}
	inline UInvenSlot* GetInvenSlot(EInvenSlotType InSlotType) {
		return InventoryComponent ? InventoryComponent->GetInvenSlot(InSlotType) : nullptr;
	}
	inline void BindOnGoldChange(const FOnPlayerGoldChange::FDelegate& Delegate) {
		if (InventoryComponent) InventoryComponent->OnGoldChange.Add(Delegate);
	}

	inline bool CanBuyItem(class UItemDataAsset* InItemDataAsset, int32 InItemCount) const {
		if (InventoryComponent)
			return InventoryComponent->CanBuyItem(InItemDataAsset, InItemCount);
		else
			return false;
	}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent = nullptr;

};
