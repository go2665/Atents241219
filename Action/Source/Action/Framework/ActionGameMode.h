// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ActionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AActionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AActionGameMode();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class UDropItemFactoryComponent* DropItemFactory = nullptr;
};
