// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryWidget->CloseInventory();
	ShopWidget->Close();
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

void UMainWidget::ShowShop(bool bShow)
{
	if (ShopWidget)
	{
		if (bShow)
		{
			ShopWidget->Open();
			InventoryWidget->OpenInventory();
		}
		else
		{
			ShopWidget->Close();
		}
	}
}
