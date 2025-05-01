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
	// 버튼 초기화 함수(버튼 인덱스, 버튼 기본 이미지, 가격)
	void OnInitialize(int32 InIndex, UTexture* InImage, int32 InCost);

	void UpdateButtonState(int32 InGold);


	// 버튼의 크기를 리턴하는 함수
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBuilder")
	FLinearColor BuildEnableColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);		// 타워건설 가능 색상

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBuilder")
	FLinearColor BuildDisableColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);		// 타워건설 불가능 색상

private:
	// 버튼의 인덱스
	int32 Index = 0;	

	// 이 버튼 사용에 필요한 금액
	int32 Cost = 0;	
};
