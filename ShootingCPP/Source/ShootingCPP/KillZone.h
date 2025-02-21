// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

UCLASS()
class SHOOTINGCPP_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnKillZoneOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BorderTop = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BorderBottom = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BorderLeft = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BorderRight = nullptr;

};
