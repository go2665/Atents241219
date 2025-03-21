// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "Action/Inventory/InvenSlot.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::InitializeItemSlot(int32 InItemSlotIndex, UInvenSlot* InSlotData)
{
	ItemSlotIndex = InItemSlotIndex; 
	SlotData = InSlotData;
	SlotData->OnSlotUpdated.AddDynamic(this, &UItemSlotWidget::RefreshSlot);

	RefreshSlot();
}

void UItemSlotWidget::RefreshSlot()
{
	UItemDataAsset* Data = SlotData->GetItemDataAsset();
	if (Data)
	{
		ItemIcon->SetBrushFromTexture(Data->ItemIcon);
		ItemIcon->SetBrushTintColor(FLinearColor::White);

		CountText->SetText(FText::AsNumber(SlotData->GetItemCount()));
		MaxCountText->SetText(FText::AsNumber(SlotData->GetMaxStackCount()));

		CountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		SeparatorText->SetVisibility(ESlateVisibility::HitTestInvisible);
		MaxCountText->SetVisibility(ESlateVisibility::HitTestInvisible);

		ShowSlotVisibility();
	}
	else
	{
		ClearSlotWidget();
	}
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Left Mouse Button Down"));
		OnSlotClicked.Broadcast(ItemSlotIndex);

		Reply = FReply::Handled();	// 이벤트 처리 완료
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Right Mouse Button Down"));
		OnSlotRClicked.Broadcast(ItemSlotIndex);
		Reply = FReply::Handled();	// 이벤트 처리 완료
	}

	return Reply;
}

void UItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Enter (%d)"), ItemSlotIndex);
	OnSlotEnter.Broadcast(ItemSlotIndex);
}

void UItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Leave (%d)"), ItemSlotIndex);
	OnSlotLeave.Broadcast();
}

void UItemSlotWidget::ShowSlotVisibility()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UItemSlotWidget::ClearSlotWidget()
{
	ItemIcon->SetBrushFromTexture(nullptr);
	ItemIcon->SetBrushTintColor(FLinearColor::Black);
	CountText->SetVisibility(ESlateVisibility::Hidden);
	SeparatorText->SetVisibility(ESlateVisibility::Hidden);
	MaxCountText->SetVisibility(ESlateVisibility::Hidden);
}

