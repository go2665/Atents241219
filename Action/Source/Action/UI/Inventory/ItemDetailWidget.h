// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UItemDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Open(class UItemDataAsset* InData);
	void Close();

	inline void SetParentCanvasSlot(class UCanvasPanelSlot* InSlot) { ParentCanvasSlot = InSlot; };

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateLocation();
	void CalculateParentOrigin();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemDetail", meta = (BindWidget))
	class UImage* ItemIconImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemDetail", meta = (BindWidget))
	class UTextBlock* ItemNameTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemDetail", meta = (BindWidget))
	class UTextBlock* ItemPriceTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemDetail", meta = (BindWidget))
	class UTextBlock* ItemDescriptionTextBlock;


private:
	class APlayerController* PlayerController = nullptr;

	class UCanvasPanelSlot* CanvasSlot = nullptr;
	class UCanvasPanelSlot* ParentCanvasSlot = nullptr;

	// 부모 기준의 원점(부모의 좌상단 위치를 원점으로 사용, 변하지 않는다고 가정)
	FVector2D ParentOrigin;

	bool bIsCalculatedParentOrigin = false;

};
