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
	virtual void ShowSlotVisibility() override;
	virtual void ClearSlotWidget() override;

	inline bool IsEmpty() const { return SlotData->IsEmpty(); }

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateLocation();

	class APlayerController* PlayerController = nullptr;
};
