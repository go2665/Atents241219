// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EInvenSlotType.h"
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
	void UseItem(uint8 InSlotIndex);

	// 특정 슬롯에 있는 무기 아이템 장비
	void EquipItem(uint8 InSlotIndex);

	// 인벤토리에 아이템 추가
	bool AddItem(class UItemDataAsset* InItemDataAsset);

	// 인벤토리에 있는 아이템 옮기기
	void MoveItem(EInvenSlotType InFromSlot, EInvenSlotType InToSlot);

	// 인벤토리의 특정 슬롯 반환
	inline InvenSlotBase* GetInvenSlot(EInvenSlotType InSlotType) {
		if (static_cast<uint8>(InSlotType) < MaxSlotCount)	// 일반 슬롯을 요구할 경우
		{
			return &InvenSlots[static_cast<uint8>(InSlotType)];
		}
		else if (InSlotType == EInvenSlotType::Temporary)	// 임시 슬롯을 요구할 경우
		{
			return &TempSlot;
		}
		else if (InSlotType == EInvenSlotType::Weapon)	// 무기 슬롯을 요구할 경우
		{
			return &WeaponSlot;
		}
		return nullptr;
	};

	// 테스트용 함수
	void TestPrintInventory();
	
private:
	// 비어있는 슬롯 반환
	InvenSlotBase* GetEmptySlot();

	// 비어있는 스롯에 아이템 추가
	bool AddItemToEmptySlot(class UItemDataAsset* InItemDataAsset);

	// 적절한 인덱스인지 확인하는 함수
	inline bool IsValidIndex(uint8 InSlotIndex) {
		return InSlotIndex < MaxSlotCount;
	}

	// 최대 슬롯 갯수
	static const int8 MaxSlotCount = 4;

	// 인벤토리의 모든 슬롯을 저장하는 배열
	InvenSlotBase InvenSlots[MaxSlotCount];

	// 임시 슬롯
	InvenSlotBase TempSlot;

	// 무기 슬롯
	InvenSlotBase WeaponSlot;

	class AActionPlayerCharacter* Owner = nullptr;

};
