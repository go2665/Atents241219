// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DEPRECATED_TowerUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API U_DEPRECATED_TowerUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpgradeWidgetInitialize(class ATowerBaseActor* Tower);
	
	UFUNCTION()
	void OpenUpgradeWidget();
	
	UFUNCTION()
	void CloseUpgradeWidget();

	void Test_PrintTower();

	UFUNCTION()
	void SellTower();

protected:
	UFUNCTION()
	void UpgradeTower();
	
	
	void UpdateButtonState();

private:
	UFUNCTION()
	void OnCloseAnimationFinished();


protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeCostText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Open = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Close = nullptr;

	UPROPERTY()
	class ATowerBaseActor* OwnerTower = nullptr;

	UPROPERTY()
	class ATowerDefenceGameMode* GameMode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	FLinearColor UpgradeEnableColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);		// 업그레이드 가능 색상

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	FLinearColor UpgradeDisableColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);	// 업그레이드 불가능 색상

};
