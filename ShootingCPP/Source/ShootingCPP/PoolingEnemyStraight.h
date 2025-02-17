// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "PoolingEnemyStraight.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemyStraight : public APoolingEnemyBase
{
	GENERATED_BODY()

public:
	APoolingEnemyStraight();
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UStaticMeshComponent* CoreMesh = nullptr;
};
