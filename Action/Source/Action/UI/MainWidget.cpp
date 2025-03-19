// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryWidget->CloseInventory();
}

void UMainWidget::ShowInventory(bool bShow)
{
	if (InventoryWidget)
	{
		if (bShow)
		{
			InventoryWidget->RefreshInventory();
			InventoryWidget->OpenInventory();
		}
		else
		{
			InventoryWidget->CloseInventory();
		}
	}
}
