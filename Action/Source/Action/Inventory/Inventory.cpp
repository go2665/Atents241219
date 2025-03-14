// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Item/ItemData/ItemDataAsset.h"
#include "../Item/Interface/UsableItem.h"
#include "../Item/Interface/EquipableItem.h"
#include "../Player/ActionPlayerCharacter.h"
#include "../Framework/ActionGameMode.h"

void Inventory::Initialize(AActionPlayerCharacter* InOwner)
{
	// 반드시 BeginPlay에서 호출되어야 함

	for (int i = 0; i < MaxSlotCount; ++i)
	{
		InvenSlots[i].SetSlotIndex(i);	// 순서대로 슬롯 인덱스 설정
	}

	Owner = InOwner;
}

void Inventory::UseItem(uint8 InSlotIndex)
{
	if (IsValidIndex(InSlotIndex))
	{
		InvenSlotBase* Slot = GetInvenSlot(static_cast<EInvenSlotType>(InSlotIndex));
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

void Inventory::EquipItem(uint8 InSlotIndex)
{
	if (IsValidIndex(InSlotIndex))
	{
		InvenSlotBase* Slot = GetInvenSlot(static_cast<EInvenSlotType>(InSlotIndex));
		if (!Slot->IsEmpty())
		{
			UItemDataAsset* Data = Slot->GetItemDataAsset();

			if (WeaponSlot.GetItemDataAsset() != Data)	// 다른 아이템일 경우에만 실행
			{
				IEquipableItem* EquipableItem = Cast<IEquipableItem>(Data);
				if (EquipableItem)	// 장비 가능한 아이템인 경우에만 실행
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
					//	FString::Printf(TEXT("Equip Item(%d) : %s"), InSlotIndex, *Data->ItemName.ToString()));
									
					EquipableItem->EquipItem(Owner);	// 오너에게 장비
					MoveItem(static_cast<EInvenSlotType>(InSlotIndex), EInvenSlotType::Weapon);	// 슬롯 위치 바꾸기
				}
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

void Inventory::MoveItem(EInvenSlotType InFromSlot, EInvenSlotType InToSlot)
{
	InvenSlotBase* FromSlot = GetInvenSlot(InFromSlot);
	InvenSlotBase* ToSlot = GetInvenSlot(InToSlot);

	if (!FromSlot->IsEmpty())	// FromSlot에는 무조건 아이템이 있어야 함
	{
		if (ToSlot->IsEmpty())	
		{
			// ToSlot이 비어있을 경우
			ToSlot->SetItemDataAsset(FromSlot->GetItemDataAsset(), FromSlot->GetItemCount());
			FromSlot->ClearSlot();
		}
		else 
		{
			// ToSlot이 비어있지 않을 경우

			if (FromSlot->GetItemDataAsset() == ToSlot->GetItemDataAsset()
				&& FromSlot->GetItemDataAsset()->IsStackable())
			{
				// 같은 종류의 아이템이면서 스택이 가능한 아이템인 경우(ToSlot은 FromSlot과 같은 아이템이므로 따로 체크 안함)
				UItemDataAsset* ItemData = FromSlot->GetItemDataAsset();
				int32 MaxCount = ItemData->ItemStackCount - ToSlot->GetItemCount();	// ToSlot에 최대로 들어갈 수 있는 개수
				int32 MergeCount = FMath::Min(MaxCount, FromSlot->GetItemCount());	// FromSlot에 있는 아이템 개수와 MaxCount 중 작은 것을 선택
								
				FromSlot->DecreaseItemCount(MergeCount);	// FromSlot에서 MergeCount만큼 감소				
				ToSlot->IncreaseItemCount(MergeCount);		// ToSlot에서 MergeCount만큼 증가
			}
			else
			{
				// 다른 종류의 아이템이면 그냥 스왑				
				UItemDataAsset* TempItemData = ToSlot->GetItemDataAsset();
				int32 TempItemCount = ToSlot->GetItemCount();
				ToSlot->SetItemDataAsset(FromSlot->GetItemDataAsset(), FromSlot->GetItemCount());
				FromSlot->SetItemDataAsset(TempItemData, TempItemCount);
			}
		}
	}
}

int32 Inventory::SellItem(uint8 InSlotIndex)
{
	// InSlotIndex 슬롯에 있는 아이템을 판매하고 얻는 골드 반환	

	int32 SellPrice = 0;
	InvenSlotBase* Slot = GetInvenSlot(static_cast<EInvenSlotType>(InSlotIndex));
	if (!Slot)
	{
		UItemDataAsset* Data = Slot->GetItemDataAsset();	
		if (!Data)
		{
			SellPrice = 0.5f * Data->ItemPrice * Slot->GetItemCount();	// 판매 금액은 Price의 절반 * 개수
			Slot->ClearSlot();	// 슬롯 비우기
		}
	}

	return SellPrice;
}

void Inventory::TestPrintInventory()
{
	// 인벤토리에 포션3개, 사과3개, 바나나2개가 있을 경우 아래처럼 출력(화면과 로그 모두 출력)
	// [0] : 포션 x 3, [1] : 사과 x 3, [2] : 바나나 x 2

	FString PrintString = TEXT("");
	for (int i = 0; i < MaxSlotCount; i++)
	{
		UItemDataAsset* ItemData = InvenSlots[i].GetItemDataAsset();
		FString Item;
		if (ItemData)
		{
			Item = FString::Printf(TEXT("[%d] : %s x %d, "), i, *ItemData->ItemName.ToString(), InvenSlots[i].GetItemCount());			
		}
		else
		{
			Item = FString::Printf(TEXT("[%d] : Empty, "), i);
		}
		PrintString += Item;
	}

	UItemDataAsset* TempItemData = TempSlot.GetItemDataAsset();
	FString TempItem;
	if (TempItemData)
	{
		TempItem = FString::Printf(TEXT("[Temp] : %s x %d, "), *TempItemData->ItemName.ToString(), TempSlot.GetItemCount());
	}
	else
	{
		TempItem = FString::Printf(TEXT("[Temp] : Empty, "));
	}
	PrintString += TempItem;

	UItemDataAsset* WeaponItemData = WeaponSlot.GetItemDataAsset();
	FString WeaponItem;
	if (WeaponItemData)
	{
		WeaponItem = FString::Printf(TEXT("[Weapon] : %s, "), *WeaponItemData->ItemName.ToString());
	}
	else
	{
		WeaponItem = FString::Printf(TEXT("[Weapon] : Empty, "));
	}
	PrintString += WeaponItem;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PrintString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PrintString);

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
