// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlotWidget.h"
#include "Action/Inventory/InvenSlot.h"
#include "TempSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UTempSlotWidget : public UItemSlotWidget
{
	GENERATED_BODY()
	
public:
	void UpdataLocation(FVector2D InLocation);

	inline bool IsEmpty() const { return SlotData->IsEmpty(); }
};
