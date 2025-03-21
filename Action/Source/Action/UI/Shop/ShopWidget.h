// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 상점 열기
	UFUNCTION(BlueprintCallable)
	void Open();

	// 상점 닫기
	UFUNCTION(BlueprintCallable)
	void Close();

protected:
	virtual void NativeConstruct() override;

protected:
	// 상점 아이템 위젯들에 데이터를 세팅한다.
	void InitializeShopItemWidgets();

	// 구매 버튼 상태 갱신
	void UpdateBuyButtonsState();

	// ShopItemDataTables에서 랜덤하게 데이터 테이블을 리턴
	UDataTable* GetRandomShopItemDataTable();

private:
	UFUNCTION()
	void OnMoneyChanged(int32 NewMoney);

	// 상점이 열려있는지 확인
	inline bool IsOpen() const { return CurrentDataTable != nullptr; }

protected:
	// 상점 아이템 데이터 테이블들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	TArray<UDataTable*> ShopItemDataTables;

	UPROPERTY()
	// 상점 아이템 위젯 목록
	TArray<UShopItemWidget*> ShopItemWidgets;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (WidgetBind))
	class UVerticalBox* ShopItems;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (WidgetBind))
	class UButton* Exit;

private:
	UPROPERTY()
	UDataTable* CurrentDataTable = nullptr;
};