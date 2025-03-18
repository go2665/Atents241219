// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "ItemSlotWidget.h"
#include "Action/Player/ActionPlayerState.h"
#include "Components/CanvasPanelSlot.h"


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
						ItemSlotWidget->OnSlotClicked.AddDynamic(this, &UInventoryWidget::OnSlotClicked);
						ItemSlotWidgets.Add(ItemSlotWidget);
					}
				}

				TempSlotWidget->InitializeItemSlot(static_cast<int32>(EInvenSlotType::Temporary),
					PlayerState->GetInvenSlot(EInvenSlotType::Temporary));
				TempSlotWidget->SetParentCanvasSlot(Cast<UCanvasPanelSlot>(Slot));
			}
		}
	}
}

void UInventoryWidget::RefreshInventory()
{
	for (auto& ItemSlotWidget : ItemSlotWidgets)
	{
		ItemSlotWidget->RefreshSlot();
	}
}

void UInventoryWidget::OnSlotClicked(int32 InSlotIndex)
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
}