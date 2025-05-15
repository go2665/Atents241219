// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/SplineComponent.h"
#include "TowerDefence/Enemy/Enemy.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetMobility(EComponentMobility::Static);
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	ensure(WaveDataAsset);
	
	ElapsedTime = 0.0f;
	NextWaveIndex = 0;
	bFinalWave = false;

	GameMode = GetWorld()->GetAuthGameMode<ATowerDefenceGameMode>();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	if (!bFinalWave && (ElapsedTime >= WaveDataAsset->WaveDatas[NextWaveIndex].StartTime))
	{
		StartWave(NextWaveIndex);
		NextWaveIndex++;

		if (NextWaveIndex >= WaveDataAsset->WaveDatas.Num())
		{
			bFinalWave = true;
		}
	}
}

int32 AEnemySpawner::GetTotalEnemyDamage() const
{
	int32 TotalDamage = 0;
	for (const FWaveData& WaveData : WaveDataAsset->WaveDatas)
	{
		for (const FEnemyGroupData& GroupData : WaveData.EnemyGroups)
		{
			if (GroupData.EnemyClass)
			{
				AEnemy* Enemy = GroupData.EnemyClass.GetDefaultObject();
				if (Enemy)
				{
					TotalDamage += (Enemy->GetDamage() * GroupData.SpawnCount);
				}
			}
		}
	}
	return TotalDamage;
}

int32 AEnemySpawner::GetTotalEnemyCount() const
{
	int32 TotalCount = 0;
	for (const FWaveData& WaveData : WaveDataAsset->WaveDatas)
	{
		for (const FEnemyGroupData& GroupData : WaveData.EnemyGroups)
		{
			TotalCount += GroupData.SpawnCount;
		}
	}
	return TotalCount;
}

void AEnemySpawner::StartWave(int32 InWaveIndex)
{
	if (bShowDebugInfo)
	{
		// 웨이브 시작 시간과 인덱스 로그로 출력
		UWorld* World = GetWorld();
		int Minutes = FMath::FloorToInt(World->TimeSeconds / 60);
		float Seconds = FMath::Fmod(World->TimeSeconds, 60.0f);
		FString TimeString = FString::Printf(TEXT("%02d:%05.3f"), Minutes, Seconds);
		UE_LOG(LogTemp, Warning, TEXT("[%s] StartWave : [%d]!"), *TimeString, InWaveIndex);
	}

	FWaveData& WaveData = WaveDataAsset->WaveDatas[InWaveIndex];
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	for (const FEnemyGroupData& GroupData : WaveData.EnemyGroups)
	{
		if (GroupData.SpawnInterval > 0)
		{
			// 순차적으로 스폰 => 타이머 사용						
			FTimerHandle& TimerHandle = SpawnTimerMap.FindOrAdd(&GroupData);
			TimerManager.SetTimer(
				TimerHandle,
				FTimerDelegate::CreateUObject(this, &AEnemySpawner::RepeatSpawnEnemy, &GroupData),
				GroupData.SpawnInterval,
				true);
		}
		else
		{
			// SpawnCount 만큼 한번에 스폰
			for (int i = 0; i < GroupData.SpawnCount; i++)
			{
				SpawnEnemy(GroupData.EnemyClass);
			}
		}
	}
	
}

void AEnemySpawner::SpawnEnemy(TSubclassOf<AEnemy> InEnemyClass)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AEnemy* Enemy = World->SpawnActor<AEnemy>(
			InEnemyClass,
			SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World),
			SplineComponent->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World));

		Enemy->InitializeEnemy(SplineComponent);
		Enemy->OnEnemyAttack.AddUObject(GameMode, &ATowerDefenceGameMode::SubtractHealth);
		Enemy->OnEnemyKilled.AddUObject(GameMode, &ATowerDefenceGameMode::OnEnemyKilled);
	}
}

void AEnemySpawner::RepeatSpawnEnemy(const FEnemyGroupData* InGroupData)
{
	if (bShowDebugInfo)
	{
		UWorld* World = GetWorld();
		int Minutes = FMath::FloorToInt(World->TimeSeconds / 60);
		float Seconds = FMath::Fmod(World->TimeSeconds, 60.0f);
		FString TimeString = FString::Printf(TEXT("%02d:%05.3f"), Minutes, Seconds);
		UE_LOG(LogTemp, Warning, TEXT("[%s] RepeatSpawnEnemy: %s"), *TimeString, *InGroupData->EnemyClass->GetName());
	}

	int32& Count = SpawnCountMap.FindOrAdd(InGroupData);
	if (Count < InGroupData->SpawnCount)
	{
		Count++;
		SpawnEnemy(InGroupData->EnemyClass);
	}
	else
	{
		// SpawnCount 만큼 스폰했으면 타이머 종료
		FTimerHandle* InHandle = SpawnTimerMap.Find(InGroupData);
		if (InHandle)
		{
			GetWorld()->GetTimerManager().ClearTimer(*InHandle);
			SpawnCountMap.Remove(InGroupData);
			SpawnTimerMap.Remove(InGroupData);
		}
	}
}

