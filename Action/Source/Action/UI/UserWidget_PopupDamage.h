// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_PopupDamage.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_PopupDamage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void PlayPopupAnimation(float Damage);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "PopupDamage", meta = (BindWidget))
	class UTextBlock* DamageText;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "PopupDamage", meta = (BindWidgetAnim))
	UWidgetAnimation* Popup;


};
