// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "ItemDataAsset_Heal.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UItemDataAsset_Heal : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Heal")
	float HealAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Heal")
	float HealDuration = -1.0f;

};
