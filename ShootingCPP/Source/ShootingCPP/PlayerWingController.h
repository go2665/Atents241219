// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerWingController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APlayerWingController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerWingController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void OnPlayerDie();

	UFUNCTION()
	void GeoInputRestart(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* GameOverMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* GeoRestartAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level = nullptr;
};
