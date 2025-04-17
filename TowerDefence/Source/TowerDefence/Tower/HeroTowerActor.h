// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerBaseActor.h"
#include "TowerDefence/Skill/SkillDataAsset.h"
#include "HeroTowerActor.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AHeroTowerActor : public ATowerBaseActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseSkill(FVector InLocation);

private:
	void FindActorsInRadius(FVector Center, float Radius, TArray<AEnemyBase*>& OutActors);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<USkillDataAsset*> SkillDataAssets;

	
};
