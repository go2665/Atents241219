// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerUpgradeWidget : public UUserWidget
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
	class UButton* CloseButton = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Open = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Close = nullptr;

	UPROPERTY()
	class ATowerBaseActor* OwnerTower = nullptr;

};
