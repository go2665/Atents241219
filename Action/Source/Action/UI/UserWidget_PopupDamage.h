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
	// 데미지 텍스트 설정 및 Popup 애니메이션 재생
	UFUNCTION()
	void ActivateWidget(float Damage);

protected:
	// 데미지 텍스트 위젯
	UPROPERTY(BlueprintReadOnly, Category = "PopupDamage", meta = (BindWidget))
	class UTextBlock* DamageText;

	// Popup 애니메이션
	UPROPERTY(BlueprintReadOnly, Transient, Category = "PopupDamage", meta = (BindWidgetAnim))
	UWidgetAnimation* Popup;


};
