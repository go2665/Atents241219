// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnerBaseComponent.h"
#include "SpawnerAreaComponent.h"
#include "SpawnerDiagonalComponent.h"
#include "SpawnerMassComponent.h"
#include "Spawner.generated.h"

// 스포너 컴포넌트들과 스폰될 영역들을 관리하는 액터 클래스
UCLASS()
class SHOOTINGCPP_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:

private:
	void AddSpawnerBaseComponent();
	void AddSpawnerAreaComponent();
	void AddSpawnerDiagonalComponent();
	void AddSpawnerMassComponent();

protected:
	// 스폰될 영역 컴포넌트(기본)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnAreaBase = nullptr;

	// 스포너 컴포넌트(기본)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	USpawnerBaseComponent* SpawnerBaseType = nullptr;

	// 스폰될 영역 컴포넌트(일정 범위 안 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnAreaArea = nullptr;

	// 스포너 컴포넌트(일정 범위 안 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	USpawnerAreaComponent* SpawnerAreaType = nullptr;

	// 스폰될 영역 컴포넌트(대각선으로 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnAreaDiagonal = nullptr;

	// 스포너 컴포넌트(대각선으로 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	USpawnerDiagonalComponent* SpawnerDiagonalType = nullptr;

	// 스폰될 영역 컴포넌트(대각선으로 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnAreaMass = nullptr;

	// 스포너 컴포넌트(대각선으로 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	USpawnerMassComponent* SpawnerMassType = nullptr;

};
