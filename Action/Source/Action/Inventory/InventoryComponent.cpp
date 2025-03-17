// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Action/Player/ActionPlayerCharacter.h"
#include "Action/Framework/ActionGameMode.h"
#include "Action/Item/EItemType.h"
#include "Action/Item/FDropItemDataTableRow.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Action/Item/Interface/UsableItem.h"
#include "Action/Item/Interface/EquipableItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 인벤토리 슬롯들 초기화(반드시 모든 EInvenSlotType에 대해 슬롯이 있어야 한다.)
	InvenSlots.Reserve(MaxSlotCount + 2);	// 일반슬롯10개 + 임시슬롯1개 + 무기슬롯1개
	for (int i = 0; i < 10; i++)
	{
		UInvenSlot* Slot = CreateDefaultSubobject<UInvenSlot>(FName(*FString::Printf(TEXT("Slot_%d"), i)));
		Slot->Initialize(i);
		InvenSlots.Add(static_cast<EInvenSlotType>(i), Slot);
	}

	UInvenSlot* TempSlot = CreateDefaultSubobject<UInvenSlot>(FName(TEXT("TempSlot")));
	InvenSlots.Add(EInvenSlotType::Temporary, TempSlot);

	UInvenSlot* WeaponSlot = CreateDefaultSubobject<UInvenSlot>(FName(TEXT("WeaponSlot")));
	InvenSlots.Add(EInvenSlotType::Weapon, WeaponSlot);
}

bool UInventoryComponent::AddItem(UItemDataAsset* InItemDataAsset)
{
	bool bIsSuccess = false;

	if (InItemDataAsset)	// InItemDataAsset가 nullptr이 아니면 실행
	{
		if (InItemDataAsset->IsStackable())	// 스택 가능한 아이템인지 확인
		{
			for (int i = 0; i < MaxSlotCount; i++)
			{
				UInvenSlot* Slot = GetInvenSlot(static_cast<EInvenSlotType>(i));
				if (Slot->GetItemDataAsset() == InItemDataAsset					// 같은 종류의 아이템이고
					&& Slot->GetItemCount() < InItemDataAsset->ItemStackCount)	// 빈칸이 있다.
				{
					Slot->IncreaseItemCount();	// 빈칸이 있을 경우는 갯수만 증가
					bIsSuccess = true;

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
						FString::Printf(TEXT("Increase Stack Item(%d) : %s [ %d / %d ]"),
							Slot->GetSlotIndex(), *InItemDataAsset->ItemName.ToString(),
							Slot->GetItemCount(), InItemDataAsset->ItemStackCount));
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

void UInventoryComponent::MoveItem(EInvenSlotType InFromSlot, EInvenSlotType InToSlot)
{
	UInvenSlot* FromSlot = GetInvenSlot(InFromSlot);
	UInvenSlot* ToSlot = GetInvenSlot(InToSlot);

	if (!FromSlot->IsEmpty())	// FromSlot에는 무조건 아이템이 있어야 함
	{
		if (ToSlot->IsEmpty())
		{
			// ToSlot이 비어있을 경우
			ToSlot->SetItem(FromSlot->GetItemDataAsset(), FromSlot->GetItemCount());
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

				FromSlot->AddItemCount(-MergeCount);	// FromSlot에서 MergeCount만큼 감소
				ToSlot->AddItemCount(MergeCount);		// ToSlot에서 MergeCount만큼 증가				
			}
			else
			{
				// 다른 종류의 아이템이면 그냥 스왑				
				UItemDataAsset* TempItemData = ToSlot->GetItemDataAsset();
				int32 TempItemCount = ToSlot->GetItemCount();
				ToSlot->SetItem(FromSlot->GetItemDataAsset(), FromSlot->GetItemCount());
				FromSlot->SetItem(TempItemData, TempItemCount);

				//Swap(FromSlot, ToSlot);	// FromSlot과 ToSlot을 스왑
			}
		}
	}
}

int32 UInventoryComponent::SellItem(EInvenSlotType InSlot)
{
	// InSlotIndex 슬롯에 있는 아이템을 판매하고 얻는 골드 반환	
	int32 SellPrice = 0;
	UInvenSlot* Slot = GetInvenSlot(InSlot);
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

void UInventoryComponent::UseItem(EInvenSlotType InSlot)
{
	UInvenSlot* Slot = GetInvenSlot(InSlot);
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

void UInventoryComponent::EquipItem(EInvenSlotType InSlot)
{
	UInvenSlot* Slot = GetInvenSlot(InSlot);
	if (!Slot->IsEmpty())
	{
		UItemDataAsset* Data = Slot->GetItemDataAsset();
		UInvenSlot* WeaponSlot = GetInvenSlot(EInvenSlotType::Weapon);

		if (WeaponSlot->GetItemDataAsset() != Data)	// 다른 아이템일 경우에만 실행
		{
			IEquipableItem* EquipableItem = Cast<IEquipableItem>(Data);
			if (EquipableItem)	// 장비 가능한 아이템인 경우에만 실행
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
				//	FString::Printf(TEXT("Equip Item(%d) : %s"), InSlotIndex, *Data->ItemName.ToString()));

				EquipableItem->EquipItem(Owner);			// 오너에게 장비
				MoveItem(InSlot, EInvenSlotType::Weapon);	// 슬롯 위치 바꾸기
			}
		}
	}
}

void UInventoryComponent::TestPrintInventory()
{
	// 인벤토리에 포션3개, 사과3개, 바나나2개가 있을 경우 아래처럼 출력(화면과 로그 모두 출력)
	// [0] : 포션 x 3, [1] : 사과 x 3, [2] : 바나나 x 2, [3] : Empty, ..., [Temp] : Empty, [Weapon] : Empty

	FString PrintString = TEXT("");
	for (int i = 0; i < MaxSlotCount; i++)
	{
		UInvenSlot* InvenSlot = GetInvenSlot(static_cast<EInvenSlotType>(i));
		if (InvenSlot)
		{
			UItemDataAsset* ItemData = InvenSlot->GetItemDataAsset();
			FString Item;
			if (ItemData)
			{
				Item = FString::Printf(TEXT("[%d] : %s x %d, "), i, *ItemData->ItemName.ToString(), InvenSlot->GetItemCount());
			}
			else
			{
				Item = FString::Printf(TEXT("[%d] : Empty, "), i);
			}
			PrintString += Item;
		}
	}

	UInvenSlot* TempSlot = GetInvenSlot(EInvenSlotType::Temporary);
	UItemDataAsset* TempItemData = TempSlot->GetItemDataAsset();
	FString TempItem;
	if (TempItemData)
	{
		TempItem = FString::Printf(TEXT("[Temp] : %s x %d, "), *TempItemData->ItemName.ToString(), TempSlot->GetItemCount());
	}
	else
	{
		TempItem = FString::Printf(TEXT("[Temp] : Empty, "));
	}
	PrintString += TempItem;

	UItemDataAsset* WeaponItemData = GetInvenSlot(EInvenSlotType::Weapon)->GetItemDataAsset();
	FString WeaponItem;
	if (WeaponItemData)
	{
		WeaponItem = FString::Printf(TEXT("[Weapon] : %s"), *WeaponItemData->ItemName.ToString());
	}
	else
	{
		WeaponItem = FString::Printf(TEXT("[Weapon] : Empty"));
	}
	PrintString += WeaponItem;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PrintString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PrintString);
}

void UInventoryComponent::TestInventoryAddDefaultItems(UDataTable* TestTable)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AActionGameMode* GameMode = World->GetAuthGameMode<AActionGameMode>();

		if (TestTable)
		{
			TMap<FName, uint8*> RowMap = TestTable->GetRowMap();
			for (auto& Elem : RowMap)
			{
				FDropItemDataTableRow* Row = (FDropItemDataTableRow*)Elem.Value;

				for (int i = 0; i < Row->Count; ++i)	// Count만큼 아이템 생성
				{
					UItemDataAsset* Data = GameMode->GetItemDataAsset(Row->ItemType);
					AddItem(Data);
				}
			}
		}
		else
		{
			// 데이터 테이블이 없으면 인벤토리에 사과10개, 도끼 1개, 사과3개
			UItemDataAsset* Data = GameMode->GetItemDataAsset(EItemType::Apple);
			for (int i = 0; i < 10; i++)
			{
				AddItem(Data);
			}

			Data = GameMode->GetItemDataAsset(EItemType::Axe);
			AddItem(Data);

			Data = GameMode->GetItemDataAsset(EItemType::Apple);
			for (int i = 0; i < 3; i++)
			{
				AddItem(Data);
			}
		}
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(PlayerController->GetPawn());
	Owner = Player;

	OnGoldChange.AddDynamic(this, &UInventoryComponent::TestPrintGoldChange);	// 골드 변화 델리게이트에 함수 추가
}

UInvenSlot* UInventoryComponent::GetEmptySlot()
{
	UInvenSlot* EmptySlot = nullptr;
	for (int i = 0; i < MaxSlotCount; ++i)
	{
		// InvenSlots 0~9는 반드시 있어야함.없을 경우 터지는게 나음
		UInvenSlot* InvenSlot = GetInvenSlot(static_cast<EInvenSlotType>(i));
		if (InvenSlot->IsEmpty())
		{
			EmptySlot = InvenSlot;
			break;	// 찾았으면 루프 종료
		}
	}
	return EmptySlot;
}

bool UInventoryComponent::AddItemToEmptySlot(UItemDataAsset* InItemDataAsset)
{
	bool bIsSuccess = false;
	UInvenSlot* EmptySlot = GetEmptySlot();
	if (EmptySlot)	// 빈칸이 있으면 실행
	{
		EmptySlot->SetItem(InItemDataAsset);
		bIsSuccess = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//	FString::Printf(TEXT("Add Item to Empty Slot(%d) : %s"),
		//		EmptySlot->GetSlotIndex(), *InItemDataAsset->ItemName.ToString()));
	}

	return bIsSuccess;
}
