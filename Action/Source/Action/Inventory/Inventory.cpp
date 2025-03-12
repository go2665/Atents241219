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

	// 스택 가능한 아이템인지 확인
	//	- 스택 가능하면 같은 아이템이 있는지 확인하고 해당 슬롯에 추가
	//		- 넘치면 다음 빈 슬롯에 추가
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
