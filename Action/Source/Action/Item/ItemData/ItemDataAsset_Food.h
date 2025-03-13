// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "../Interface/UsableItem.h"
#include "ItemDataAsset_Food.generated.h"

/**
 * 체력을 일정 시간 간격으로 정해진 회수만큼 회복시키는 음식 아이템
 */
UCLASS()
class ACTION_API UItemDataAsset_Food : public UItemDataAsset, public IUsableItem
{
	GENERATED_BODY()

public:
	virtual void UseItem(AActor* Target) override;

public:
	// 회복량(틱 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Food")
	float HealAmount = 5.0f;

	// 틱 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Food", meta = (ClampMin = "0.1"))
	float TickInterval = 0.2f;

	// 틱 회수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Food", meta = (ClampMin = "1"))
	int32 TickCount = 5;
};
