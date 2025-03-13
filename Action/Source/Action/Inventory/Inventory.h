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
	// 초기화 함수(슬롯 인덱스 설정)
	void Initialize(class AActionPlayerCharacter* InOwner);

	// 특정 슬롯에 있는 아이템 사용
	void UseItem(int8 InSlotIndex);

	// 인벤토리에 아이템 추가
	bool AddItem(class UItemDataAsset* InItemDataAsset);

	// 인벤토리의 특정 슬롯 반환
	inline InvenSlotBase* GetInvenSlot(int8 InSlotIndex) { return &InvenSlots[InSlotIndex]; }

	// 테스트용 함수
	void TestPrintInventory();
	
private:
	// 비어있는 슬롯 반환
	InvenSlotBase* GetEmptySlot();

	// 비어있는 스롯에 아이템 추가
	bool AddItemToEmptySlot(class UItemDataAsset* InItemDataAsset);

	// 적절한 인덱스인지 확인하는 함수
	inline bool IsValidIndex(int8 InSlotIndex) {
		return 0 <= InSlotIndex && MaxSlotCount > InSlotIndex;
	}

	// 최대 슬롯 갯수
	static const int8 MaxSlotCount = 4;

	// 인벤토리의 모든 슬롯을 저장하는 배열
	InvenSlotBase InvenSlots[MaxSlotCount];

	class AActionPlayerCharacter* Owner = nullptr;

};
