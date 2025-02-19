// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "PoolingEnemy_SmallDebris.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_SmallDebris : public APoolingEnemyBase
{
	GENERATED_BODY()
	
public:
	APoolingEnemy_SmallDebris();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

private:
	UFUNCTION()
	void SetRandomDirection();

protected:
	UPROPERTY(EditAnywhere, Category = "Pooling Enemy")
	float RotatingSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Pooling Enemy")
	FVector RotateAxis = FVector::UpVector;

	UPROPERTY(EditAnywhere, Category = "Pooling Enemy")
	float RandomDirectionIntervalMin = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Pooling Enemy")
	float RandomDirectionIntervalMax = 5.0f;

	UPROPERTY()
	FTimerHandle RandomDirectionTimer;
};
