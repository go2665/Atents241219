// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "ProjectileNoPool.generated.h"

UCLASS()
class SHOOTINGCPP_API AProjectileNoPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileNoPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Explosion();


public:	

protected:
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* VisualEffect = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LifeTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* ExplosionEffect = nullptr;

	

};
