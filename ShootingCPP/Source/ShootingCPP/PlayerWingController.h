// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

private:
	UFUNCTION()
	void OnPlayerDie();
};
