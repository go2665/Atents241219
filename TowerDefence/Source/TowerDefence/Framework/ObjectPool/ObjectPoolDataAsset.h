// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "ObjectPoolDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Object Pool")
	EPooledActorType Type;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Object Pool")
	TSubclassOf<AActor> ObjectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Object Pool")
	int32 PoolSize;
};

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UObjectPoolDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Object Pool")
	TArray<FObjectPoolData> ObjectPoolData;
};
