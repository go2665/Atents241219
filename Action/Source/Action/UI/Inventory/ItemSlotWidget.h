// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, InItemSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotEnter, int32, InItemSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotLeave);


/**
 * 
 */
UCLASS()
class ACTION_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 초기화용
	virtual void InitializeItemSlot(int32 InItemSlotIndex, class UInvenSlot* InSlotData);

	// SlotData를 기반으로 UI 갱신
	UFUNCTION()
	virtual void RefreshSlot();

	// Delegate
	FOnSlotClicked OnSlotClicked;
	FOnSlotClicked OnSlotRClicked;
	FOnSlotEnter OnSlotEnter;
	FOnSlotLeave OnSlotLeave;

	// Getters
	inline int32 GetItemSlotIndex() const { return ItemSlotIndex; }
	inline class UInvenSlot* GetSlotData() const { return SlotData; }

	// Setters

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	virtual void ShowSlotVisibility();
	virtual void ClearSlotWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	int32 ItemSlotIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UTextBlock* CountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UTextBlock* SeparatorText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UTextBlock* MaxCountText;

	UPROPERTY()
	class UInvenSlot* SlotData = nullptr;
};
