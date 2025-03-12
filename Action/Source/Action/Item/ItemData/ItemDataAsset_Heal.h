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
	// 전체 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Heal")
	float HealAmount = 10.0f;

	// 회복 지속 시간(회복 시간이 0보다 작으면 즉시 회복)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Heal")
	float HealDuration = -1.0f;

};
