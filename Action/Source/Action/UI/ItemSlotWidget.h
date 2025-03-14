// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// SlotData를 기반으로 UI 갱신
	void RefreshSlot();

	// 초기화용
	inline void InitializeItemSlot(int32 InItemSlotIndex, class InvenSlotBase* InSlotData) { 
		ItemSlotIndex = InItemSlotIndex; SlotData = nullptr; 
	}

	// Getters
	inline int32 GetItemSlotIndex() const { return ItemSlotIndex; }
	inline class InvenSlotBase* GetSlotData() const { return SlotData; }

	// Setters

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	int32 ItemSlotIndex = -1;


	class InvenSlotBase* SlotData = nullptr;
};
