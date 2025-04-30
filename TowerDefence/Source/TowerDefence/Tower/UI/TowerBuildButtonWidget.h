// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "TowerBuildButtonWidget.generated.h"

class USizeBox;
class UButton;
class UTextBlock;

DECLARE_DELEGATE_OneParam(FOnBuildButtonClicked, int32);

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuildButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnInitialize(int32 InIndex, UTexture* InImage, int32 InCost);

	//inline UButton* GetBuildButton() const { return BuildButton; }

	inline FVector2D GetButtonSize() const
	{
		if (SizeBox)
		{
			return FVector2D(SizeBox->GetWidthOverride(), SizeBox ->GetHeightOverride());
		}
		return FVector2D::ZeroVector;
	}

private:
	// 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnButtonClicked();

public:
	// 버튼 클릭 시 호출되는 델리게이트
	FOnBuildButtonClicked OnBuildButtonClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* BuildButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PriceText = nullptr;

private:
	int32 Index = 0;	// 버튼의 인덱스
};
