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
	void InputInventory(const FInputActionValue& Value);
	void InputTest(const FInputActionValue& Value);

private:
	UFUNCTION()
	void OnPostHudBeginPlay();

	UFUNCTION()
	void OnInventoryOpen(bool bIsOpen);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputMappingContext* MovementContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputAction* LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputAction* SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputAction* RollAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputAction* AttackAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|Movement")
	class UInputAction* TestAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|UI")
	class UInputMappingContext* UserInterfaceContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Setup|UI")
	class UInputAction* InventoryAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Setup")
	float CameraPitchMin = -40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Setup")
	float CameraPitchMax = 30.0f;

private:
	class AActionPlayerCharacter* PlayerCharacter = nullptr;
	class AMainHUD* MainHUD;

	int32 MovementInputPriority = 1;
	int32 UserInterfaceInputPriority = 0;

	class UEnhancedInputLocalPlayerSubsystem* InputSystem = nullptr;
};

