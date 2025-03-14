// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "Action/Inventory/InvenSlotBase.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Components/Image.h"

void UItemSlotWidget::InitializeItemSlot(int32 InItemSlotIndex, InvenSlotBase* InSlotData)
{
	ItemSlotIndex = InItemSlotIndex; 
	SlotData = InSlotData;
	SlotData->OnSlotUpdated = std::bind(&UItemSlotWidget::RefreshSlot, this);	
}

void UItemSlotWidget::RefreshSlot()
{
	UItemDataAsset* Data = SlotData->GetItemDataAsset();
	if (Data)
	{
		ItemIcon->SetBrushFromTexture(Data->ItemIcon);
		ItemIcon->SetBrushTintColor(FLinearColor::White);
	}
	else
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemIcon->SetBrushTintColor(FLinearColor::Black);
	}
}

