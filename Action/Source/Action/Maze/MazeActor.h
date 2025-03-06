// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeActor.generated.h"

UCLASS()
class ACTION_API AMazeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze", meta = (ClmapMin = "1", ClampMax = "100"))
	int32 Width = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze", meta = (ClmapMin = "1", ClampMax = "100"))
	int32 Height = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze")
	int32 RandomSeed = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze")
	TSubclassOf<class ACellActor> CellActorClass = nullptr;
};
