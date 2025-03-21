// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ANormalEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	ANormalEnemy();

	UFUNCTION(BlueprintCallable)
	void Attack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxWalkSpeed = 400.0f;
};
