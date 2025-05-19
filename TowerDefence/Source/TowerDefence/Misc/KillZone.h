// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

UCLASS()
class TOWERDEFENCE_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AKillZone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

};
