// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "GameModeCPP.h"
#include "PoolingEnemy_Debris.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_Debris : public APoolingEnemyBase
{
	GENERATED_BODY()
	
public:
	APoolingEnemy_Debris();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	void SpawnSmallDebris();


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float OrbitRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float RotatingSpeed = 360.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	int32 RandomSmallDebrisCountMin = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	int32 RandomSmallDebrisCountMax = 8;

private:
	UPROPERTY()
	AGameModeCPP* GameModeCPP = nullptr;
};
