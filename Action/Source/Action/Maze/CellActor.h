// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellActor.generated.h"

UCLASS()
class ACTION_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;



protected:
	// 배경 매시(벽과 바닥)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell")
	class UStaticMeshComponent* BackgroundMesh = nullptr;

	// 문 매시의 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell")
	TArray<class UStaticMeshComponent*> GateMeshArray;

	class CellData* CellData = nullptr;
};
