// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Action/Inventory/EInvenSlotType.h"
#include "ItemSlotWidget.h"
#include "TempSlotWidget.h"
#include "EquipSlotWidget.h"

#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	void RefreshInventory();
	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnInvenSlotClicked(int32 InSlotIndex);

	UFUNCTION()
	void OnEquipSlotClicked(int32 InSlotIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UUniformGridPanel* ItemSlotsGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	UTempSlotWidget* TempSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	UEquipSlotWidget* EquipSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UTextBlock* GoldTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UImage* SellIcon;
	
	UPROPERTY()
	TArray<UItemSlotWidget*> ItemSlotWidgets;

private:

	class AActionPlayerState* PlayerState = nullptr;

	UFUNCTION()
	void RefreshGoldText(int32 NewGold);

	UFUNCTION()
	void OnSellIconClicked();
};
