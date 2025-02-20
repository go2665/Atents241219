// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerBaseComponent.h"
#include "SpawnerAreaComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class SHOOTINGCPP_API USpawnerAreaComponent : public USpawnerBaseComponent
{
	GENERATED_BODY()
	
protected:
	virtual FTransform GetSpawnTransform() const override;
};
