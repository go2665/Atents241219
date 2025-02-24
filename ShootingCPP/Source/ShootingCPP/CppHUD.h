// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CppShooterUserWidget.h"
#include "CppHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API ACppHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UCppShooterUserWidget* ShooterUserWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCppShooterUserWidget> ShooterUserWidgetClass = nullptr;
};
