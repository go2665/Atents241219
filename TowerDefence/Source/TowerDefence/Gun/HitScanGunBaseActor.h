// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBaseActor.h"
#include "HitScanGunBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AHitScanGunBaseActor : public AGunBaseActor
{
	GENERATED_BODY()
	
protected:
	virtual void Shoot() override;

protected:
	// MuzzleLocation에서 Target까지 라인트레이스하는 함수
	bool LineTraceToTarget(FVector Target);
	bool LineTraceToTarget(AActor* Target);
};
