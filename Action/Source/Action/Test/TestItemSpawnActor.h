// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestItemSpawnActor.generated.h"

UCLASS()
class ACTION_API ATestItemSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestItemSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void TestSpawnItem();
	
	// 아이템 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test Data")
	class UItemDataAsset* ItemDataAsset = nullptr;

};
