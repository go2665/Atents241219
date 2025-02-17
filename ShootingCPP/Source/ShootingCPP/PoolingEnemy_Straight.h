// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "PoolingEnemy_Straight.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_Straight : public APoolingEnemyBase
{
	GENERATED_BODY()

public:
	APoolingEnemy_Straight();
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UStaticMeshComponent* CoreMesh = nullptr;
};
