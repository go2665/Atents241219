// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"

#include "TestActor_AddComponent.generated.h"

//class UNiagaraComponent;

UCLASS()
class SHOOTINGCPP_API ATestActor_AddComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor_AddComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 루트	컴포넌트
	USceneComponent* Root = nullptr;

	// 스태틱 메쉬 컴포넌트
	UPROPERTY(EditAnywhere)		// 컴포넌트는 VisibleAnywhere로 설정해도 에디터 편집이 가능하다.
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* Effect = nullptr;
};
