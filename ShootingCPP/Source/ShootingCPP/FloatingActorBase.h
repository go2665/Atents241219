// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActorBase.generated.h"

UCLASS()
class SHOOTINGCPP_API AFloatingActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActorBase();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	float MoveHeight = 100.0f;

	UPROPERTY(EditAnywhere)
	float SpinSpeed = 100.0f;

	float ElapsedTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseHeight = 40.0f;

	virtual void OnUpdateFloatMesh(float DeltaTime);
};
