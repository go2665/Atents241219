// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "EEnemyType.h"
#include "GameModeCPP.h"
#include "SpawnerBaseComponent.generated.h"


UCLASS( meta=(BlueprintSpawnableComponent) )
class SHOOTINGCPP_API USpawnerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerBaseComponent();

	// Getters and Setters
	inline UBoxComponent* GetSpawnArea() const { return SpawnArea; }
	inline void SetSpawnArea(UBoxComponent* InSpawnArea) { SpawnArea = InSpawnArea; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//// 컴포넌트가 등록될 때 실행되는 함수
	//virtual void OnRegister() override;

	// 주기적으로 실행되면서 적을 하나 스폰하는 함수
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	virtual void Spawn();

	// 스폰될 위치를 반환해주는 함수
	UFUNCTION(BlueprintPure, Category = "Spawner")
	virtual FTransform GetSpawnTransform() const;

private:
	// 스폰될 영역 컴포넌트와 화살표를 생성하는 함수(미완성 - 사용안함)
	void AddBoxAndArrowComponent();
		
protected:
	// 스폰될 영역 컴포넌트
	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnArea = nullptr;

	// 스폰될 적의 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	EEnemyType SpawnEnemyType = EEnemyType::ET_Straight;

	// 스폰 주기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnInterval = 10000.0f;

	// 적을 꺼내오기 위해 게임모드를 캐스팅 해 놓은 변수
	AGameModeCPP* GameMode = nullptr;

//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
//	UBoxComponent* Test = nullptr;
};
