// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerBaseComponent.h"
#include "SpawnerDiagonalComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API USpawnerDiagonalComponent : public USpawnerBaseComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Spawn() override;
	virtual FTransform GetSpawnTransform() const override;

private:
	void SubSpawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SubSpawnInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SubSpawnSpacing = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 SubSpawnCount = 5;

private:
	/*mutable*/ int32 SubSpawnIndex = 0;

	UPROPERTY()
	FTimerHandle SubSpawnTimer;

};
