// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "ItemSlotWidget.h"
#include "Action/Inventory/Inventory.h"
#include "Action/Player/ActionPlayerState.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Canvas)
	{
		Canvas = NewObject<UCanvasPanel>();
	}

	if (!ItemSlotsGrid)
	{
		ItemSlotsGrid = NewObject<UUniformGridPanel>();
		Canvas->AddChild(ItemSlotsGrid);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			AActionPlayerState* PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();	

			if (PlayerState)
			{
				int32 Count = ItemSlotsGrid->GetChildrenCount();
				for (int32 i = 0; i < Count; i++)
				{
					UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(ItemSlotsGrid->GetChildAt(i));
					if (ItemSlotWidget)
					{
						ItemSlotWidget->InitializeItemSlot(i, PlayerState->GetInvenSlot(static_cast<EInvenSlotType>(i)));
						ItemSlotWidgets.Add(ItemSlotWidget);
					}
				}
			}
		}
	}

}
