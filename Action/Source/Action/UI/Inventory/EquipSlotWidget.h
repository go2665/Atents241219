// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlotWidget.h"
#include "EquipSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UEquipSlotWidget : public UItemSlotWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitializeItemSlot(int32 InItemSlotIndex, class UInvenSlot* InSlotData) override;
	virtual void RefreshSlot() override;

protected:
	virtual void ClearSlotWidget();

};
