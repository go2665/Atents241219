// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBaseActor.generated.h"

UCLASS()
class TOWERDEFENCE_API AGunBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GunMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun", meta = (ClampMin = "0.1"))
	float ScaleFactor = 2.0f;
};
