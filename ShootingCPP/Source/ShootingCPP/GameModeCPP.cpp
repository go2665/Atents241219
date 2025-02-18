// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeCPP.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPoolCpp.h"
#include "PoolingEnemy_Straight.h"
#include "PoolingEnemy_Wave.h"
#include "PoolingEnemy_Shooter.h"
#include "PoolingEnemy_Chase.h"

APoolingEnemyBase* AGameModeCPP::GetEnemy(EEnemyType Type, const FTransform& SpawnTransform)
{
	APoolingEnemyBase* result = nullptr;
	switch(Type)
	{
		case EEnemyType::ET_Straight:
			result = GetEnemy_Straight(SpawnTransform);
			break;
		case EEnemyType::ET_Wave:
			result = GetEnemy_Wave(SpawnTransform);
			break;
		case EEnemyType::ET_Shooter:
			result = GetEnemy_Shooter(SpawnTransform);
			break;
	}
	return result;
}

APoolingObjectBase* AGameModeCPP::GetBullet(const FTransform& SpawnTransform)
{
	APoolingObjectBase* result = nullptr;
	if (EnemyPool)
	{
		result = EnemyPool->GetObject_Bullet(SpawnTransform);
	}
	
	return result;
}

void AGameModeCPP::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> EnemyPools;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AEnemyPoolCpp::StaticClass(),
		EnemyPools);

	EnemyPool = Cast<AEnemyPoolCpp>(EnemyPools[0]);
}

APoolingEnemyBase* AGameModeCPP::GetEnemy_Straight(const FTransform& SpawnTransform)
{
	APoolingEnemyBase* result = nullptr;
	if (EnemyPool)
	{
		result = Cast<APoolingEnemyBase>(EnemyPool->GetObject_Straight(SpawnTransform));
	}

	return result;
}

APoolingEnemyBase* AGameModeCPP::GetEnemy_Wave(const FTransform& SpawnTransform)
{
	APoolingEnemyBase* result = nullptr;
	if (EnemyPool)
	{
		result = Cast<APoolingEnemyBase>(EnemyPool->GetObject_Wave(SpawnTransform));
	}

	return result;
}

APoolingEnemyBase* AGameModeCPP::GetEnemy_Shooter(const FTransform& SpawnTransform)
{
	APoolingEnemyBase* result = nullptr;
	if (EnemyPool)
	{
		result = Cast<APoolingEnemyBase>(EnemyPool->GetObject_Shooter(SpawnTransform));
	}

	return result;
}

APoolingEnemyBase* AGameModeCPP::GetEnemy_Chase(const FTransform& SpawnTransform)
{
	APoolingEnemyBase* result = nullptr;
	if (EnemyPool)
	{
		result = Cast<APoolingEnemyBase>(EnemyPool->GetObject_Chase(SpawnTransform));
	}

	return result;
}
