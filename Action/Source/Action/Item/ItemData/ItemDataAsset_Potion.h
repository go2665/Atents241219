// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "../Interface/UsableItem.h"
#include "ItemDataAsset_Potion.generated.h"

/**
 * 사용자의 체력을 퍼센트 단위로 즉시 회복시키는 아이템
 */
UCLASS()
class ACTION_API UItemDataAsset_Potion : public UItemDataAsset, public IUsableItem
{
	GENERATED_BODY()

public:
	// 포션 회복량(% 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Potion", meta = (ClampMin = "0", ClampMax = "1"))
	float HealAmount = 0.3f;

	virtual void UseItem(AActor* Target) override;
};
