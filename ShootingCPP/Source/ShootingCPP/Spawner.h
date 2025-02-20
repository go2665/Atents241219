// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnerBaseComponent.h"
#include "SpawnerAreaComponent.h"
#include "Spawner.generated.h"

// 스포너 커모넌트들과 스폰될 영역들을 관리	하는 액터 클래스
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

protected:
	// 스폰될 영역 컴포넌트(기본)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnAreaBase = nullptr;

	// 스포너 컴포넌트(기본)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	USpawnerBaseComponent* SpawnerBase = nullptr;

	// 스폰될 영역 컴포넌트(일정 범위 안 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnAreaArea = nullptr;

	// 스포너 컴포넌트(일정 범위 안 스폰)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	USpawnerAreaComponent* SpawnerArea = nullptr;

};
