// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "EEnemyType.h"
#include "SpawnerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGCPP_API USpawnerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerBaseComponent();

	inline void SetSpawnArea(UBoxComponent* InSpawnArea) { SpawnArea = InSpawnArea; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void Spawn();

	UFUNCTION(BlueprintPure, Category = "Spawner")
	virtual FTransform GetSpawnTransform() const;
		
protected:
	UPROPERTY()
	UBoxComponent* SpawnArea = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	EEnemyType SpawnEnemyType = EEnemyType::ET_Straight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnInterval = 1.0f;

private:
	class AGameModeCPP* GameMode = nullptr;
};
