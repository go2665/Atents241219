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

	inline void SetParentCanvasSlot(class UCanvasPanelSlot* InSlot) { ParentCanvasSlot = InSlot; };
	inline bool IsEmpty() const { return SlotData->IsEmpty(); }	

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateLocation();
	void CalculateParentOrigin();

	class APlayerController* PlayerController = nullptr;

	class UCanvasPanelSlot* CanvasSlot = nullptr;
	class UCanvasPanelSlot* ParentCanvasSlot = nullptr;

	// 부모 기준의 원점(부모의 좌상단 위치를 원점으로 사용, 변하지 않는다고 가정)
	FVector2D ParentOrigin;

	bool bIsCalculatedParentOrigin = false;

};
