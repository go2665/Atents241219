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

	// getter
	inline int8 GetSlotIndex() const { return SlotIndex; }
	inline class UItemDataAsset* GetItemDataAsset() const { return ItemDataAsset; }

	// setter
	inline void SetSlotIndex(int8 InSlotIndex) { SlotIndex = InSlotIndex; }
	inline void SetItemDataAsset(class UItemDataAsset* InItemDataAsset) { ItemDataAsset = InItemDataAsset; }

	// check
	inline bool IsEmpty() const { return ItemDataAsset == nullptr; }

protected:
	int8 SlotIndex = -1;

	class UItemDataAsset* ItemDataAsset = nullptr;
};
