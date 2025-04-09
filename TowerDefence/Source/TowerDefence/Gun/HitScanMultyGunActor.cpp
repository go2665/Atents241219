// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanMultyGunActor.h"

void AHitScanMultyGunActor::Shoot()
{
	Super::Super::Shoot();

	// 여러마리의 적을 동시에 공격하기(들어온 순서대로 공격하기)
	HitProcess();
}
