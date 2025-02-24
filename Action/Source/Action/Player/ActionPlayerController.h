// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ActionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AActionPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void InputMove(const FInputActionValue& Value);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputMappingContext* DefaultContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* MoveAction = nullptr;

private:
	class ActionPlayerCharacter* PlayerCharacter = nullptr;
};

