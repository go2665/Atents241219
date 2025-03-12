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

bool Inventory::AddItem(UItemDataAsset* InItemDataAsset)
{
	bool bIsSuccess = false;
	InvenSlotBase* EmptySlot = GetEmptySlot();
	if(EmptySlot)
	{
		EmptySlot->SetItemDataAsset(InItemDataAsset);
		bIsSuccess = true;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("AddItem(%d) : %s"), EmptySlot->GetSlotIndex(), *InItemDataAsset->ItemName.ToString()));
	}

	//return EmptySlot != nullptr;	// 빈슬롯이 있었으면 true, 없었으면 false 반환
	return bIsSuccess;
}

InvenSlotBase* Inventory::GetEmptySlot()
{
	InvenSlotBase* EmptySlot = nullptr;
	for (int i = 0; i < MaxSlotCount; ++i)
	{
		if (InvenSlots[i].IsEmpty())
		{
			EmptySlot = &InvenSlots[i];
			break;	// 찾았으면 루프 종료
		}
	}
	return EmptySlot;
}
