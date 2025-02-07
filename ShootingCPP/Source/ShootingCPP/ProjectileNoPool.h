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

	// 이 총알이 터질 때 호출되는 함수
	void Explosion();

private:	
	// 이 총알이 다른 액터와 겹칠 때 호출되는 함수
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// 이 총알이 기본적으로 보여주는 이펙트
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* VisualEffect = nullptr;

	// 이 총알의 충돌을 감지하는 구체 컴포넌트(충돌 영역)
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	// 이 총알의 이동을 담당하는 컴포넌트(직선 이동)
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	// 이 총알의 수명(수명이 다되면 Explosion 함수를 호출하여 폭발)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LifeTime = 3.0f;

	// 이 총알을 맞은 대상에게 주는 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 5.0f;

	// 이 총알이 폭발할 때 사용하는 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* ExplosionEffect = nullptr;
};
