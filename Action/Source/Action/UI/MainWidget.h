// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Action/UI/Inventory/InventoryWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)	
	void ShowInventory(bool bShow);

	UFUNCTION(BlueprintCallable)
	inline void ToggleInventory() { if (InventoryWidget) InventoryWidget->ToggleInventory(); };

	inline FOnInventoryOpen& GetInventoryOpenDelegate() { return InventoryWidget->OnInventoryOpen; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	UInventoryWidget* InventoryWidget;
};
