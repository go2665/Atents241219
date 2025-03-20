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
	void SetItemDataAsset(class UItemDataAsset* NewItemDataAsset);

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnItemCountTextChanged(const FText& InText);

	UFUNCTION()
	void OnItemCountTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

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

private:
	class AActionPlayerState* PlayerState = nullptr;
	const int32 MinimumItemCount = 1;
};

// 상점 아이템 위젯
// 1. ItemDataAsset을 가지고 있다.(ShopWidget에서 설정)
// 2. 아이템 이미지, 이름, 가격, 설명을 보여준다.
// 3. ItemCount를 설정할 수 있다.
//   3.1. ItemCount는 0보다 커야 한다.
//   3.2. ItemCount만큼 구매가 불가능할 경우 구매 버튼을 비활성화한다.(돈이 부족하거나 인벤토리 공간이 부족할 경우)
// 4. 구매 버튼을 누르면 인벤토리에 아이템을 추가하고 구매 금액만큼 돈을 뺀다.