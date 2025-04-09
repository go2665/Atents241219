// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanGunBaseActor.h"
#include "HitScanNearMultyGunActor.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AHitScanNearMultyGunActor : public AHitScanGunBaseActor
{
	GENERATED_BODY()
	
protected:
	virtual void Shoot() override;
};
