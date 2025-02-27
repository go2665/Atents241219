// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Player/ActionPlayerCharacter.h"
#include "TestSingleWeaponCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ATestSingleWeaponCharacter : public AActionPlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Test")
	TSubclassOf<class AWeaponActor> TestWeaponClass = nullptr;

	UPROPERTY()
	class AWeaponActor* TestWeapon = nullptr;
	
};
