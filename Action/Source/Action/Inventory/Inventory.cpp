// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Item/ItemData/ItemDataAsset.h"
#include "../Item/Interface/UsableItem.h"
#include "../Player/ActionPlayerCharacter.h"

void Inventory::Initialize(AActionPlayerCharacter* InOwner)
{
	// 반드시 BeginPlay에서 호출되어야 함

	for (int i = 0; i < MaxSlotCount; ++i)
	{
		InvenSlots[i].SetSlotIndex(i);	// 순서대로 슬롯 인덱스 설정
	}

	Owner = InOwner;
}

void Inventory::UseItem(int8 InSlotIndex)
{
	if (IsValidIndex(InSlotIndex))
	{
		InvenSlotBase* Slot = GetInvenSlot(InSlotIndex);
		if (!Slot->IsEmpty())
		{
			UItemDataAsset* Data = Slot->GetItemDataAsset();
			IUsableItem* UsableItem = Cast<IUsableItem>(Data);
			if (UsableItem)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
				//	FString::Printf(TEXT("Use Item(%d) : %s"), InSlotIndex, *Data->ItemName.ToString()));
				UsableItem->UseItem(Owner);
				Slot->DecreaseItemCount();
			}
		}
	}
}

bool Inventory::AddItem(UItemDataAsset* InItemDataAsset)
{
	bool bIsSuccess = false;

	if (InItemDataAsset)	// InItemDataAsset가 nullptr이 아니면 실행
	{
		if (InItemDataAsset->IsStackable())	// 스택 가능한 아이템인지 확인
		{
			for (int i = 0; i < MaxSlotCount; i++)
			{
				if (InvenSlots[i].GetItemDataAsset() == InItemDataAsset					// 같은 종류의 아이템이고
					&& InvenSlots[i].GetItemCount() < InItemDataAsset->ItemStackCount)	// 빈칸이 있다.
				{
					InvenSlots[i].IncreaseItemCount();	// 빈칸이 있을 경우는 갯수만 증가
					bIsSuccess = true;

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
						FString::Printf(TEXT("Increase Stack Item(%d) : %s [ %d / %d ]"), 
							InvenSlots[i].GetSlotIndex(), *InItemDataAsset->ItemName.ToString(),
							InvenSlots[i].GetItemCount(), InItemDataAsset->ItemStackCount));
					break;
				}
			}

			if (!bIsSuccess)	// 모든 슬롯을 다 돌았는데 빈칸이 있는 같은 종류의 아이템이 들어있는 슬롯이 없었다.
			{
				bIsSuccess = AddItemToEmptySlot(InItemDataAsset);	// 빈칸에 아이템 추가 시도
			}
		}
		else
		{
			bIsSuccess = AddItemToEmptySlot(InItemDataAsset);		// 스택 불가능한 아이템은 빈칸에 추가
		}
	}
	
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

bool Inventory::AddItemToEmptySlot(UItemDataAsset* InItemDataAsset)
{
	bool bIsSuccess = false;
	InvenSlotBase* EmptySlot = GetEmptySlot();
	if (EmptySlot)	// 빈칸이 있으면 실행
	{
		EmptySlot->SetItemDataAsset(InItemDataAsset);
		bIsSuccess = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("Add Item to Empty Slot(%d) : %s"),
				EmptySlot->GetSlotIndex(), *InItemDataAsset->ItemName.ToString()));
	}

	return bIsSuccess;
}
