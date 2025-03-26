// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 초기 설정(현재 체력, 최대 체력)
	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void SetHealth(float InHealth, float InMaxHealth);

protected:
	virtual void NativeConstruct() override;

private:
	// 체력이 변경되었을 때 실행될 함수(델리게이트에 의해 실행)
	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void OnHealthChange(float InHealth);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HealthBar")
	float MaxHealth = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealthBar", meta = (BindWidget))
	class UProgressBar* Progress;
};
