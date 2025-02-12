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
};
