// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Tower/TowerBuilder.h"
#include "TowerDefence/Spawner/EnemySpawner.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World)
	{
		UObjectPoolSubsystem* ObjectPoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
		if (ObjectPoolSubsystem && ObjectPoolDataAsset)
		{
			ObjectPoolSubsystem->InitializeObjectPool(ObjectPoolDataAsset); // 오브젝트 풀 초기화
		}


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
			RemainingEnemyCount += Spawner->GetTotalEnemyCount();
		}
		StartHealth = FMath::Max(1, static_cast<int32>(TotalEnemyDamage * 0.7f));
		SetHealth(StartHealth); // 시작 체력 설정
		//UE_LOG(LogTemp, Warning, TEXT("Total Enemy Damage: %d"), TotalEnemyDamage);
		//UE_LOG(LogTemp, Warning, TEXT("Total Enemy Count: %d"), RemainingEnemyCount);
	}
}

void ATowerDefenceGameMode::OnEnemyKilled(int32 InGold)
{
	AddGold(InGold); // 적이 죽었을 때 골드 증가
	DecreaseRemainingEnemyCount(); // 남은 적 수 감소
}

void ATowerDefenceGameMode::GameClear()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Clear!"));
	OnGameClear.Broadcast(); // 게임 클리어 델리게이트 호출
	bIsGamePlay = false;
}

void ATowerDefenceGameMode::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
	OnGameOver.Broadcast(); // 게임 오버 델리게이트 호출
	bIsGamePlay = false;
}
