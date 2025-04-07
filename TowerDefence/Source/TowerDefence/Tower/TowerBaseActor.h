// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerBaseActor.generated.h"

UCLASS()
class TOWERDEFENCE_API ATowerBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;


protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UChildActorComponent* GunActor = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TowerBodyMesh = nullptr;

	// 일반 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	TSubclassOf<class AGunBaseActor> GunClass = nullptr;
};
