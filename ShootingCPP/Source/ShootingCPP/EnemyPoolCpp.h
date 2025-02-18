// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolingObjectBase.h"
#include "PoolingEnemyBase.h"
#include "EnemyPoolCpp.generated.h"

UCLASS()
class SHOOTINGCPP_API AEnemyPoolCpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPoolCpp() { PrimaryActorTick.bCanEverTick = false; } ;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EnemyPool")
	APoolingObjectBase* GetObject_Straight(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EnemyPool")
	APoolingObjectBase* GetObject_Wave(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EnemyPool")
	APoolingObjectBase* GetObject_Shooter(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EnemyPool")
	APoolingObjectBase* GetObject_Bullet(const FTransform& SpawnTransform);

};
