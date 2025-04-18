// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaIndicator.generated.h"

UCLASS()
class TOWERDEFENCE_API AAreaIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline void SetAreaRadius(float NewRadius) { AreaRadius = NewRadius; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* AreaMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	float AreaRadius = 100.0f;
};
