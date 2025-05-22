// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"
#include "TowerDefence/Tower/Projectile.h"

void ATestActor::TestFunction()
{
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (ObjectPoolSubsystem)
	{
		ObjectPoolSubsystem->Test();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPoolSubsystem not found"));
	}
}

void ATestActor::Test_SpawnProjectile(EPooledActorType InType, const UShotDataAsset* InShotData, const AEnemy* InTarget)
{
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (ObjectPoolSubsystem)
	{
		AProjectile* Projectile = Cast<AProjectile>(ObjectPoolSubsystem->GetObject(InType));
		Projectile->OnSpawn(
			GetActorTransform(),
			InShotData,
			InTarget,
			0, 0.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPoolSubsystem not found"));
	}
}
