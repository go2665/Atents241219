// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipSlotWidget.h"
#include "Action/Inventory/InvenSlot.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEquipSlotWidget::InitializeItemSlot(int32 InItemSlotIndex, UInvenSlot* InSlotData)
{
	Super::InitializeItemSlot(InItemSlotIndex, InSlotData);

	CountText->SetVisibility(ESlateVisibility::Collapsed);
	SeparatorText->SetVisibility(ESlateVisibility::Collapsed);
	MaxCountText->SetVisibility(ESlateVisibility::Collapsed);
}

void UEquipSlotWidget::RefreshSlot()
{
	UItemDataAsset* Data = SlotData->GetItemDataAsset();
	if (Data)
	{
		ItemIcon->SetBrushFromTexture(Data->ItemIcon);
		ItemIcon->SetBrushTintColor(FLinearColor::White);

		ShowSlotVisibility();
	}
	else
	{
		ClearSlotWidget();
	}
}

void UEquipSlotWidget::ClearSlotWidget()
{
	ItemIcon->SetBrushFromTexture(nullptr);
	ItemIcon->SetBrushTintColor(FLinearColor::Transparent);
}
