// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "ShopItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UShopItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 아이템 데이터 세팅
	UFUNCTION(BlueprintCallable)
	void SetShopItemData(class UItemDataAsset* InNewItemDataAsset, int32 InMaxCount);
	void UpdateBuyButtonState();

protected:
	virtual void NativeConstruct() override;
	
private:
	// 아이템 개수가 변경되었을 때 실행될 함수
	UFUNCTION()
	void OnItemCountTextChanged(const FText& InText);

	// 아이템 개수를 확정지었을 때 실행될 함수
	UFUNCTION()
	void OnItemCountTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

	// 구매 버튼이 클릭되었을 때 실행될 함수
	UFUNCTION()
	void OnBuyButtonClicked();

	// 재고가 다 떨어졌을 때 실행될 함수
	void OnSoldOut();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem")
	class UItemDataAsset* ItemDataAsset = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UImage* ItemIcon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UTextBlock* ItemName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UTextBlock* ItemPrice;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UTextBlock* ItemDescription;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UEditableTextBox* ItemCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UButton* ItemBuy;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UTextBlock* ItemStockCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ShopItem", meta = (WidgetBind))
	class UOverlay* SoldOut;

private:
	// 인벤토리에 접근하기 위한 플레이어 상태
	class AActionPlayerState* PlayerState = nullptr;

	// 최소 구매 개수
	const int32 MinimumItemCount = 1;

	// 재고 개수
	int32 StockCount = 0;
};