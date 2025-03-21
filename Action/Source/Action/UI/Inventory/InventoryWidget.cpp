// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "ItemSlotWidget.h"
#include "Action/Player/ActionPlayerState.h"
#include "Action/Item/Interface/EquipableItem.h"
#include "Action/Item/Interface/UsableItem.h"
#include "Action/UI/Inventory/ItemDetailWidget.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if (!Canvas)
	//{
	//	Canvas = NewObject<UCanvasPanel>();
	//}

	//if (!ItemSlotsGrid)
	//{
	//	ItemSlotsGrid = NewObject<UUniformGridPanel>();
	//	Canvas->AddChild(ItemSlotsGrid);
	//}

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();	

			if (PlayerState)
			{
				int32 Count = ItemSlotsGrid->GetChildrenCount();
				for (int32 i = 0; i < Count; i++)
				{
					UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(ItemSlotsGrid->GetChildAt(i));
					if (ItemSlotWidget)
					{
						EInvenSlotType SlotType = static_cast<EInvenSlotType>(i);
						ItemSlotWidget->InitializeItemSlot(i, PlayerState->GetInvenSlot(SlotType));
						ItemSlotWidget->OnSlotClicked.AddDynamic(this, &UInventoryWidget::OnInvenSlotClicked);						
						ItemSlotWidget->OnSlotRClicked.AddDynamic(this, &UInventoryWidget::OnInvenSlotRClicked);
						ItemSlotWidget->OnSlotEnter.AddDynamic(this, &UInventoryWidget::OnInvenSlotEnter);
						ItemSlotWidget->OnSlotLeave.AddDynamic(this, &UInventoryWidget::OnInvenSlotLeave);
						ItemSlotWidgets.Add(ItemSlotWidget);
					}
				}

				TempSlotWidget->InitializeItemSlot(static_cast<int32>(EInvenSlotType::Temporary),
					PlayerState->GetInvenSlot(EInvenSlotType::Temporary));
				TempSlotWidget->SetParentCanvasSlot(Cast<UCanvasPanelSlot>(Slot));

				EquipSlotWidget->InitializeItemSlot(static_cast<int32>(EInvenSlotType::Weapon),
					PlayerState->GetInvenSlot(EInvenSlotType::Weapon));
				EquipSlotWidget->OnSlotClicked.AddDynamic(this, &UInventoryWidget::OnEquipSlotClicked);
				EquipSlotWidget->OnSlotEnter.AddDynamic(this, &UInventoryWidget::OnInvenSlotEnter);
				EquipSlotWidget->OnSlotLeave.AddDynamic(this, &UInventoryWidget::OnInvenSlotLeave);
								
				PlayerState->GetOnGoldChange().AddDynamic(this, &UInventoryWidget::RefreshGoldText);
			}
		}
	}

	GoldTextBlock->SetText(FText::AsNumber(0));
	SellIcon->OnMouseButtonDownEvent.BindUFunction(this, FName("OnSellIconClicked"));

	CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseClicked);
	ItemDetailWidget->SetParentCanvasSlot(Cast<UCanvasPanelSlot>(Slot));
}

void UInventoryWidget::OpenInventory()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Open Inventory"));
	SetVisibility(ESlateVisibility::Visible);
	OnInventoryOpen.Broadcast(true);
}

void UInventoryWidget::CloseInventory()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Close Inventory"));
	SetVisibility(ESlateVisibility::Hidden);
	OnInventoryOpen.Broadcast(false);
}

void UInventoryWidget::ToggleInventory()
{
	if (IsVisible())
	{
		CloseInventory();	// 보이면 닫고
	}
	else
	{
		OpenInventory();	// 안보이면 열기
	}
}

void UInventoryWidget::RefreshInventory()
{
	for (auto& ItemSlotWidget : ItemSlotWidgets)
	{
		ItemSlotWidget->RefreshSlot();
	}
}

void UInventoryWidget::OnInvenSlotClicked(int32 InSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory : Slot(%d) Clicked"), InSlotIndex);

	if (TempSlotWidget->IsEmpty())
	{
		// 임시 슬롯이 비어있으면 InSlotIndex슬롯에서 임시 슬롯으로 아이템 이동
		PlayerState->MoveItemFromInventory(static_cast<EInvenSlotType>(InSlotIndex), EInvenSlotType::Temporary);
	}
	else
	{
		// 임시 슬롯이 차있으면 InSlotIndex슬롯과 임시 슬롯 아이템 교환
		PlayerState->MoveItemFromInventory(EInvenSlotType::Temporary, static_cast<EInvenSlotType>(InSlotIndex));
	}	

	ItemDetailWidget->Close();
}

void UInventoryWidget::OnInvenSlotRClicked(int32 InSlotIndex)
{
	UInvenSlot* InvenSlot = PlayerState->GetInvenSlot(static_cast<EInvenSlotType>(InSlotIndex));
	UItemDataAsset* ItemDataAsset = InvenSlot->GetItemDataAsset();
	
	if (IUsableItem* UsableItem = Cast<IUsableItem>(ItemDataAsset))	// ItemDataAsset이 IUsableItem를 상속받았으면
	{
		PlayerState->UseItemFromInventory(static_cast<EInvenSlotType>(InSlotIndex));	// 아이템 사용
	}
	else if (IEquipableItem* EquipableItem = Cast<IEquipableItem>(ItemDataAsset))	// ItemDataAsset이 IEquipableItem를 상속받았으면
	{
		PlayerState->EquipItemFromInventory(static_cast<EInvenSlotType>(InSlotIndex));	// 아이템 장비
	}

	ItemDetailWidget->Close();
}

void UInventoryWidget::OnEquipSlotClicked(int32 InSlotIndex)
{
	UInvenSlot* TempSlot = TempSlotWidget->GetSlotData();
	if (TempSlot)
	{
		UItemDataAsset* DataAsset = TempSlot->GetItemDataAsset();
		
		if (Cast<IEquipableItem>(DataAsset))	// 임시 슬롯에 있는 아이템이 장비 아이템이면 장비
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Equip Item : %s"), *DataAsset->ItemName.ToString()));
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Equip success"));
			PlayerState->EquipItemFromInventory(EInvenSlotType::Temporary);
		}
	}
}

void UInventoryWidget::RefreshGoldText(int32 NewGold)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Gold changed to %d"), NewGold));
	GoldTextBlock->SetText(FText::AsNumber(NewGold));
}

void UInventoryWidget::OnSellIconClicked()
{
	UInvenSlot* TempSlot = TempSlotWidget->GetSlotData();
	if (TempSlot && !TempSlot->IsEmpty())
	{
		UItemDataAsset* DataAsset = TempSlot->GetItemDataAsset();
		PlayerState->AddGold(DataAsset->ItemPrice * TempSlot->GetItemCount() * 0.5f);
		TempSlot->ClearSlot();
	}
}

void UInventoryWidget::OnCloseClicked()
{
	CloseInventory();
}

void UInventoryWidget::OnInvenSlotEnter(int32 InSlotIndex)
{
	UInvenSlot* ItemSlot = PlayerState->GetInvenSlot(static_cast<EInvenSlotType>(InSlotIndex));
	if (ItemSlot)
	{
		UItemDataAsset* DataAsset = ItemSlot->GetItemDataAsset();
		if (DataAsset)
		{
			ItemDetailWidget->Open(DataAsset);
		}
	}
}

void UInventoryWidget::OnInvenSlotLeave()
{
	ItemDetailWidget->Close();
}
