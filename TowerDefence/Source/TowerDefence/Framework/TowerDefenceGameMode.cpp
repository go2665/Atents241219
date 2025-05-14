// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Tower/TowerBuilder.h"
#include "TowerDefence/Spawner/EnemySpawner.h"

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World)
	{
		// 현재 레벨에 ATowerBuilder가 몇개 있는지 확인
		TArray<ATowerBuilder*> TowerBuilders;
		for (TActorIterator<ATowerBuilder> Iter(World, ATowerBuilder::StaticClass()); Iter; ++Iter)
		{
			ATowerBuilder* Builder = *Iter;
			Builder->OnTowerBuildStateChange.BindLambda(
				[this](bool bIsBuild)
				{
					if (bIsBuild)
					{
						EmptyTowerBuilderCount--;
					}
					else
					{
						EmptyTowerBuilderCount++;
					}
					//UE_LOG(LogTemp, Warning, TEXT("TowerBuilder Count : %d"), EmptyTowerBuilderCount);
				}
			);
			TowerBuilders.Add(Builder);
		}

		EmptyTowerBuilderCount = TowerBuilders.Num();
		//UE_LOG(LogTemp, Warning, TEXT("TowerBuilder Count : %d"), EmptyTowerBuilderCount);

		// 현재 레벨에 AEnemySpawner 모두 찾아서 최종 Health 계산
		int32 TotalEnemyDamage = 0;
		for (TActorIterator<AEnemySpawner> Iter(World, AEnemySpawner::StaticClass()); Iter; ++Iter)
		{
			AEnemySpawner* Spawner = *Iter;
			TotalEnemyDamage += Spawner->GetTotalEnemyDamage();
		}
		Health = FMath::Max(1, static_cast<int32>(TotalEnemyDamage * 0.7f));
		//UE_LOG(LogTemp, Warning, TEXT("Total Enemy Damage: %d"), TotalEnemyDamage);
	}
}

void ATowerDefenceGameMode::GameClear()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Clear!"));
	bIsGamePlay = false;
}

void ATowerDefenceGameMode::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
	bIsGamePlay = false;
}
