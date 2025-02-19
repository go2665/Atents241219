// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EEnemyType.h"
#include "PoolingObjectBase.h"
#include "PoolingEnemyBase.h"
#include "GameModeCPP.generated.h"

/**
 * 
 */

UCLASS()
class SHOOTINGCPP_API AGameModeCPP : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	APoolingEnemyBase* GetEnemy(EEnemyType Type, const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	APoolingObjectBase* GetBullet(const FTransform& SpawnTransform);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	class APoolingEnemyBase* GetEnemy_Straight(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	class APoolingEnemyBase* GetEnemy_Wave(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	class APoolingEnemyBase* GetEnemy_Shooter(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	class APoolingEnemyBase* GetEnemy_Chase(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	class APoolingEnemyBase* GetEnemy_Debris(const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	class APoolingEnemyBase* GetEnemy_SmallDebris(const FTransform& SpawnTransform);
	
private:
	UPROPERTY()
	class AEnemyPoolCpp* EnemyPool = nullptr;
};
