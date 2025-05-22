// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TestActor.generated.h"

class UShotDataAsset;
class AEnemy;

UCLASS()
class TOWERDEFENCE_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestFunction();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void Test_SpawnProjectile(EPooledActorType InType, const UShotDataAsset* InShotData, const AEnemy* InTarget);

};
