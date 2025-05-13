// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Tower/TowerBuilder.h"

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 현재 레벨에 ATowerBuilder가 몇개 있는지 확인
	TArray<ATowerBuilder*> TowerBuilders;
	if (UWorld* World = GetWorld())
	{
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
	}
}
