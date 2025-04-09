// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanMultyGunActor.h"
#include "TowerDefence/Enemy/EnemyBase.h"

void AHitScanMultyGunActor::Shoot()
{
	Super::Super::Shoot();

	CurrentGunData->TargetCount;

	// 여러마리의 적을 동시에 공격하기(들어온 순서대로 공격하기)
	int32 Count = FMath::Min(CurrentGunData->TargetCount, TargetEnemies.Num()); // 공격할 적의 수
	for (int32 i = 0; i < Count; i++)
	{
		bool bHit = LineTraceToTarget(TargetEnemies[i]);
		if (bHit)
		{
			// 데미지 처리

			CurrentGunData->ShotData->Damage;	// 총알의 데미지
			CurrentGunData->ShotData->AttributeType;	// 총알의 속성 타입
		}
	}
	


	// 여러마리의 적을 동시에 공격하기(가까운 순서대로 공격하기)
}
