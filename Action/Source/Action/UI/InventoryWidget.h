// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UUniformGridPanel* ItemSlotsGrid;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	//TSubclassOf<class UItemSlotWidget> ItemSlotWidgetClass;
	
	UPROPERTY()
	TArray<class UItemSlotWidget*> ItemSlotWidgets;

private:
	//static const int8 SlotCount = 10;
	//static const int8 Rows = 2;
	//static const int8 Columns = 5;

};
