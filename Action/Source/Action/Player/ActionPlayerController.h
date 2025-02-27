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
	void InputLook(const FInputActionValue& Value);
	void InputSprint(bool IsPress);
	void InputRoll(const FInputActionValue& Value);
	void InputAttack(const FInputActionValue& Value);
	void InputTest(const FInputActionValue& Value);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputMappingContext* DefaultContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* RollAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* AttackAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup")
	class UInputAction* TestAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Setup")
	float CameraPitchMin = -40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Setup")
	float CameraPitchMax = 30.0f;

private:
	class AActionPlayerCharacter* PlayerCharacter = nullptr;
};

