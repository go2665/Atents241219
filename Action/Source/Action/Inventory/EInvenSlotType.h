#pragma once
#include "CoreMinimal.h"
#include "EInvenSlotType.generated.h"

// 인벤토리 슬롯 타입. 반드시 UInventoryComponent::UInventoryComponent에서 슬롯과 연결되어야 함
UENUM(BlueprintType)
enum class EInvenSlotType : uint8
{
	Slot_0 = 0,
	Slot_1,
	Slot_2,
	Slot_3,
	Slot_4,
	Slot_5,
	Slot_6,
	Slot_7,
	Slot_8,
	Slot_9,
	Temporary = 100,
	Weapon = 101
};