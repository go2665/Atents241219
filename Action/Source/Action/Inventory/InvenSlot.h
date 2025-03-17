// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "InvenSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotUpdatedDelegate);

/**
 * 인벤토리에서 아이템을 담는 슬롯
 */
UCLASS()
class ACTION_API UInvenSlot : public UObject
{
	GENERATED_BODY()
		
public:
	// 초기화 함수(슬롯 인덱스 설정, 반드시 단 한번만 실행되어야 한다.)
	inline void Initialize(int8 InSlotIndex) { SlotIndex = InSlotIndex; }

	// 아이템 개수 증가
	inline void IncreaseItemCount() { SetItemCount(ItemCount + 1); }

	// 아이템 개수 감소
	inline void DecreaseItemCount() { SetItemCount(ItemCount - 1); }

	// 아이템 개수 변경
	inline void AddItemCount(int32 Count) { SetItemCount(ItemCount + Count); }

	// 아이템 설정(종류와 개수)
	inline void SetItem(UItemDataAsset* InItemDataAsset, int32 Count = 1) {
		if (ItemDataAsset != InItemDataAsset || ItemCount != Count)
		{
			ItemDataAsset = InItemDataAsset;
			ItemCount = Count;
			OnSlotUpdated.Broadcast();
		}
	}

	// 슬롯 비우기
	inline void ClearSlot() { ItemDataAsset = nullptr; ItemCount = 0; OnSlotUpdated.Broadcast(); }

	// getter
	inline int8 GetSlotIndex() const { return SlotIndex; }
	inline UItemDataAsset* GetItemDataAsset() const { return ItemDataAsset; }
	inline int32 GetItemCount() const { return ItemCount; }	
	inline int32 GetMaxStackCount() const { return ItemDataAsset ? ItemDataAsset->ItemStackCount : 0; }

	// check
	inline bool IsEmpty() const { return ItemDataAsset == nullptr; }


private:
	// setter
	inline void SetItemCount(int32 Count) { 
		if (Count != ItemCount)
		{
			ItemCount = Count;
			if (ItemCount <= 0)
			{
				ClearSlot();
			}
			else
			{
				OnSlotUpdated.Broadcast(); // 변경있을때만 처리하자
			}
		}
	}

public:
	// 슬롯 변화를 알리는 델리게이트(종류와 개수가 변경되면 브로드캐스트)
	FOnSlotUpdatedDelegate OnSlotUpdated;

protected:
	// 슬롯의 인덱스
	int8 SlotIndex = -1;

	// 슬롯이 현재 가지고 있는 아이템 데이터
	UItemDataAsset* ItemDataAsset = nullptr;

	// 슬롯에 들어있는 현재 아이템의 개수
	int32 ItemCount = 0;
};
