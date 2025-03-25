// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "ShopEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AShopEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
protected:
	virtual void Die() override;
};
