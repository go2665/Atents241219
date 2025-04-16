// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileGunBaseActor.h"
#include "TowerDefence/Shot/ShotProjectileBase.h"
#include "TowerDefence/Enemy/EnemyBase.h"
#include "TowerDefence/Tower/TowerBaseActor.h"

void AProjectileGunBaseActor::Shoot()
{
	Super::Shoot();

	UWorld* World = GetWorld();

	UProjectileShotDataAsset* ShotData = Cast<UProjectileShotDataAsset>(CurrentGunData->ShotData);
	if (ShotData)
	{
		TSubclassOf<AShotProjectileBase> ProjectileClass = ShotData->ProjectileClass;

		// ProjectileClass를 MuzzleLocation에 스폰
		if (World && ProjectileClass)
		{
			int32 Count = FMath::Min(CurrentGunData->TargetCount, TargetEnemies.Num()); // 공격할 적의 수
			for (int32 i = 0; i < Count; i++)
			{
				AShotProjectileBase* Projectile = World->SpawnActor<AShotProjectileBase>(
					ProjectileClass, MuzzleLocation->GetComponentTransform());
				if (Projectile)
				{
					// 발사체 데이터 초기화
					Projectile->InitializeShotData(
						TargetEnemies[i], Cast<UProjectileShotDataAsset>(CurrentGunData->ShotData));
					Projectile->SetModifiers(
						OwnerTower->GetBuffModifierValue(ETowerBuffModifier::Damage),	// 데미지 모디파이어 설정
						GetModifierForDebuff(CurrentGunData->ShotData->DebuffType));	// 디버프 모디파이어 설정

					Projectile->SetOwner(this); // 발사체의 소유자를 현재 총으로 설정
				}
			}
		}
	}
}
