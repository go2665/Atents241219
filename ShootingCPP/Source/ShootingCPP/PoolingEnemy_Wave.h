// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "Components/TimelineComponent.h"
#include "PoolingEnemy_Wave.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_Wave : public APoolingEnemyBase
{
	GENERATED_BODY()
	
public:
	APoolingEnemy_Wave();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

private:
	UFUNCTION()
	void UpdateWave(float value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float WaveWidth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float WaveSpeedRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UCurveFloat* WaveCurve = nullptr;

private:
	UPROPERTY()
	FTimeline WaveTimeline;


};
