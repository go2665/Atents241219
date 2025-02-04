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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 생성될 때나 에디터에서 변경할 때 호출됨
	virtual void OnConstruction(const FTransform& Transform) override;

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
