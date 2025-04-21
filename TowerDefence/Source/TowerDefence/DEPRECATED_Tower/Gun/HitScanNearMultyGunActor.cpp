// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanNearMultyGunActor.h"
#include "TowerDefence/Enemy/EnemyBase.h"

void AHitScanNearMultyGunActor::Shoot()
{

	// 여러마리의 적을 동시에 공격하기(가까운 순서대로 공격하기)

	// 적을 거리의 오름차순으로 정렬하기
	FVector GunLocation = GetActorLocation();
	TargetEnemies.Sort([GunLocation](const AEnemyBase& A, const AEnemyBase& B)
		{
			float DistanceSquaredA = FVector::DistSquared(GunLocation, A.GetActorLocation());
			float DistanceSquaredB = FVector::DistSquared(GunLocation, B.GetActorLocation());
			return DistanceSquaredA < DistanceSquaredB;	// 오름차순 정렬
		});
	
	Super::Shoot();	// 리셋 처리만 있기 때문에 마지막에 호출
}
