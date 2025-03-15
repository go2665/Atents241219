// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 더 이상 사용되지 않음. InvenSlot으로 변경됨.
 */
class ACTION_API InvenSlotBase
{
public:
	virtual ~InvenSlotBase() = default;

	// 아이템 개수 증가(넘치지 않는 경우에만 사용해야 함)
	inline void IncreaseItemCount(int32 Count = 1) { ItemCount += Count; RunDelegate(); }

	// 아이템 개수 감소(음수가 되지 않는 경우에만 사용해야 함)
	inline void DecreaseItemCount(int32 Count = 1) { 
		ItemCount -= Count; 
		if (ItemCount <= 0)
		{
			ClearSlot();
		}
		RunDelegate();
	}

	// 슬롯 비우기
	inline void ClearSlot() { ItemDataAsset = nullptr; ItemCount = 0; RunDelegate(); }

	// getter
	inline int8 GetSlotIndex() const { return SlotIndex; }
	inline class UItemDataAsset* GetItemDataAsset() const { return ItemDataAsset; }
	inline int32 GetItemCount() const { return ItemCount; }

	// setter
	inline void SetSlotIndex(int8 InSlotIndex) { SlotIndex = InSlotIndex; }
	inline void SetItemDataAsset(class UItemDataAsset* InItemDataAsset, int32 Count = 1) { 
		ItemDataAsset = InItemDataAsset; 
		ItemCount = Count;
		RunDelegate();
	}

	// check
	inline bool IsEmpty() const { return ItemDataAsset == nullptr; }

	// operator
	InvenSlotBase& operator=(const InvenSlotBase& Other)
	{
		if (this == &Other)
		{
			return *this;	// 자기 자신과 대입이면 그냥 리턴
		}

		ItemDataAsset = Other.ItemDataAsset;
		ItemCount = Other.ItemCount;
		RunDelegate();

		return *this;
	}

private:
	inline void RunDelegate() { if (OnSlotUpdated) OnSlotUpdated(); }

public:
	std::function<void()> OnSlotUpdated;

protected:
	// 슬롯의 인덱스
	int8 SlotIndex = -1;

	// 슬롯이 현재 가지고 있는 아이템 데이터
	class UItemDataAsset* ItemDataAsset = nullptr;

	// 슬롯에 들어있는 현재 아이템의 개수
	int32 ItemCount = 0;
};
