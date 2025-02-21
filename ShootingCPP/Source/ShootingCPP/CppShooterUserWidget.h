// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CppShooterUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API UCppShooterUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RefreshScore(int32 Point);

	UFUNCTION(BlueprintCallable)
	void ShowGameOverPanel();
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* GameOverPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	float MinimumIncreaseSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	float IncreaseSpeedModifier = MinimumIncreaseSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float HealthPercent = 1.0f;

private:
	int32 TargetScore = 0;
	float CurrentScore = 0.0f;

	class APlayerWingUsePool* Player = nullptr;
};
