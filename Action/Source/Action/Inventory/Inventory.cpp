// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Item/ItemDataAsset.h"

void Inventory::Initialize()
{
	for (int i = 0; i < MaxSlotCount; ++i)
	{
		InvenSlots[i].SetSlotIndex(i);
	}
}

void Inventory::AddItem(UItemDataAsset* InItemDataAsset)
{
	InvenSlotBase* EmptySlot = GetEmptySlot();
	if(EmptySlot)
	{
		EmptySlot->SetItemDataAsset(InItemDataAsset);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("AddItem(%d) : %s"), EmptySlot->GetSlotIndex(), *InItemDataAsset->ItemName.ToString()));
	}
}

InvenSlotBase* Inventory::GetEmptySlot()
{
	InvenSlotBase* EmptySlot = nullptr;
	for (int i = 0; i < MaxSlotCount; ++i)
	{
		if (InvenSlots[i].IsEmpty())
		{
			EmptySlot = &InvenSlots[i];
		}
	}
	return EmptySlot;
}
