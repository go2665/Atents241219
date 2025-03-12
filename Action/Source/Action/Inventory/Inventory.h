// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvenSlotBase.h"

/**
 * 
 */
class ACTION_API Inventory
{
public:
	void Initialize();

	void AddItem(class UItemDataAsset* InItemDataAsset);

	inline InvenSlotBase* GetInvenSlot(int8 InSlotIndex) { return &InvenSlots[InSlotIndex]; }
	
private:
	InvenSlotBase* GetEmptySlot();

	static const int8 MaxSlotCount = 10;
	InvenSlotBase InvenSlots[MaxSlotCount];

};
