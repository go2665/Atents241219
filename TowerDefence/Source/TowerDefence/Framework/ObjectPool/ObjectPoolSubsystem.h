// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolDataAsset.h"
#include "ObjectPoolSubsystem.generated.h"

class IPoolableActor;

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializeObjectPool(const UObjectPoolDataAsset* InData);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetObject(EPooledActorType InType);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReleaseObject(TScriptInterface<IPoolableActor> InObject);


	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void Test();

private:
	TMap<EPooledActorType, TArray<AActor*>> Pool;
	TMap<EPooledActorType, TArray<AActor*>> ActivatedObjects;
	TMap<EPooledActorType, TSubclassOf<AActor>> ObjectClass;

	const FVector DefaultSpawnLocation = FVector::DownVector * 10000.0f; // 기본 스폰 위치
};
