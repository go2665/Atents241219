// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeExitActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMazeExitActorBeginOverlap);


UCLASS()
class ACTION_API AMazeExitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeExitActor();

	UFUNCTION(BlueprintCallable)
	void ActivateExit(class ACellActor* InClearActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnMazeExitBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:	
	FOnMazeExitActorBeginOverlap OnGameClear;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* Effect;

};
