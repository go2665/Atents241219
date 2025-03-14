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
	// 초기화용
	void InitializeItemSlot(int32 InItemSlotIndex, class InvenSlotBase* InSlotData);

	// SlotData를 기반으로 UI 갱신
	void RefreshSlot();


	// Getters
	inline int32 GetItemSlotIndex() const { return ItemSlotIndex; }
	inline class InvenSlotBase* GetSlotData() const { return SlotData; }

	// Setters

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	int32 ItemSlotIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UImage* ItemIcon;

	class InvenSlotBase* SlotData = nullptr;
};
