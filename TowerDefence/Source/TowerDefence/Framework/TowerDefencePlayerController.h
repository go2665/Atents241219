// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TowerDefencePlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMouseClickDelegate);	// 마우스 클릭 델리게이트

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ATowerDefencePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATowerDefencePlayerController();

protected:
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void OnMouseClick(const FInputActionValue& Value);	// 마우스 클릭 이벤트 핸들러

public:
	FOnMouseClickDelegate OnMouseClickInput;	// 마우스 클릭 델리게이트

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext = nullptr;	// 기본 입력 매핑 컨텍스트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ClickAction = nullptr;	// 클릭 액션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction = nullptr;	// 무브 액션

};
