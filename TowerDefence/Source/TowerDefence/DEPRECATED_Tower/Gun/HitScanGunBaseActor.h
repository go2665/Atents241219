// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBaseActor.h"
#include "HitScanGunBaseActor.generated.h"

/**
 * 히트 스캔 총기 기본 클래스(들어온 순서대로 우선 공격)
 */
UCLASS()
class TOWERDEFENCE_API AHitScanGunBaseActor : public AGunBaseActor
{
	GENERATED_BODY()
	
protected:
	virtual void Shoot() override;

protected:
	// MuzzleLocation에서 Target까지 라인트레이스하는 함수
	bool LineTraceToTarget(FVector InTarget, TArray<AEnemyBase*>& OutHitTargets);
	bool LineTraceToTarget(AActor* InTarget, TArray<AEnemyBase*>& OutHitTargets);

	// 명중 처리(데미지와 디버프 처리)
	void HitProcess();
};
