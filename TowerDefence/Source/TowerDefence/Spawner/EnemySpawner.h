// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "TowerDefence/Spawner/Data/WaveDataAsset.h"
#include "EnemySpawner.generated.h"

class USplineComponent;
class ATowerDefenceGameMode;

UCLASS()
class TOWERDEFENCE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetTotalEnemyDamage() const;
	int32 GetTotalEnemyCount() const;	

private:
	void StartWave(int32 InWaveIndex);

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	void SpawnEnemy(EPooledActorType InEnemyType, const FVector& InOffset);
		
	void RepeatSpawnEnemy(const FEnemyGroupData* InGroupData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent = nullptr;

	// 시작 위치 이펙트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* StartVFX = nullptr; 

	// 도착 위치 이펙트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* EndVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
	UWaveDataAsset* WaveDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
	bool bShowDebugInfo = false; // 디버그 정보 출력 여부

private:
	float ElapsedTime = 0.0f;
	int32 NextWaveIndex = 0;
	bool bFinalWave = false;

	// 스폰 반복 회수를 기록할 맵
	TMap<const FEnemyGroupData*, int32> SpawnCountMap;

	// 타이머 핸들을 기록할 맵
	TMap<const FEnemyGroupData*, FTimerHandle> SpawnTimerMap;

	UPROPERTY()
	ATowerDefenceGameMode* GameMode = nullptr; // 게임 모드 인스턴스
	
};
