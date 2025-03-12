// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACTION_API InvenSlotBase
{
public:
	virtual ~InvenSlotBase() = default;

	inline class UItemDataAsset* GetItemDataAsset() const { return ItemDataAsset; }
	inline void SetItemDataAsset(class UItemDataAsset* InItemDataAsset) { ItemDataAsset = InItemDataAsset; }

protected:
	int8 SlotIndex = -1;

	class UItemDataAsset* ItemDataAsset = nullptr;
};
