// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWing.h"
#include "ObjectPoolActorComponent.h"
#include "PlayerWingUsePool.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APlayerWingUsePool : public APlayerWing
{
	GENERATED_BODY()
	
public:
	APlayerWingUsePool();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnFireStart(EProjectileType Type) override;
	virtual void OnFireEnd(EProjectileType Type) override;

	const FTransform GetFireTransform() const;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Object Pool")
	UObjectPoolActorComponent* NormalProjectilePool = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Object Pool")
	UObjectPoolActorComponent* HomingProjectilePool = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Object Pool")
	UObjectPoolActorComponent* AreaProjectilePool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Interval_Normal = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Interval_Homing = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Interval_Area = 1.0f;

	UPROPERTY()
	FTimerHandle TimerHandle_Normal;

	UPROPERTY()
	FTimerHandle TimerHandle_Homing;

	UPROPERTY()
	FTimerHandle TimerHandle_Area;

	float FireCoolTime_Normal = Interval_Normal;
	float FireCoolTime_Homing = Interval_Homing;
	float FireCoolTime_Area = Interval_Area;
};
