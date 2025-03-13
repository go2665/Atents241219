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

	// 아이템 개수 증가
	inline void IncreaseItemCount() { ++ItemCount; }

	// 아이템 개수 감소
	inline void DecreaseItemCount() { 
		--ItemCount; 
		if (ItemCount <= 0)
		{
			ClearSlot();
		}
	}

	// getter
	inline int8 GetSlotIndex() const { return SlotIndex; }
	inline class UItemDataAsset* GetItemDataAsset() const { return ItemDataAsset; }
	inline int32 GetItemCount() const { return ItemCount; }

	// setter
	inline void SetSlotIndex(int8 InSlotIndex) { SlotIndex = InSlotIndex; }
	inline void SetItemDataAsset(class UItemDataAsset* InItemDataAsset, int32 Count = 1) { 
		ItemDataAsset = InItemDataAsset; 
		ItemCount = Count;
	}

	// check
	inline bool IsEmpty() const { return ItemDataAsset == nullptr; }

protected:
	void ClearSlot() { ItemDataAsset = nullptr; ItemCount = 0; }

protected:
	// 슬롯의 인덱스
	int8 SlotIndex = -1;

	// 슬롯이 현재 가지고 있는 아이템 데이터
	class UItemDataAsset* ItemDataAsset = nullptr;

	// 슬롯에 들어있는 현재 아이템의 개수
	int32 ItemCount = 0;
};
