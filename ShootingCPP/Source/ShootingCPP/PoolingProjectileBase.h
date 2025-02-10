// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingObjectBase.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PoolingProjectileBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SHOOTINGCPP_API APoolingProjectileBase : public APoolingObjectBase
{
	GENERATED_BODY()
	
public:
	APoolingProjectileBase();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	virtual void BeginPlay() override;

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	// 총알이 충돌했을 때 호출되는 함수(터지는 처리 및 데미지 처리)
	virtual void OnExplosion();

private:
	// 총알이 다른 액터와 겹칠 때 호출되는 함수
	UFUNCTION()
	void OnComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

protected:
	// 총알 바디를 표현하기 위한 이팩트 용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	UNiagaraComponent* ProjectileVfx = nullptr;

	// 총알의 충돌 영역을 설정하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	USphereComponent* Collision = nullptr;

	// 총알의 이동을 담당하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	UProjectileMovementComponent* Movement = nullptr;

	// 총알의 수명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	float LifeTime = 3.0f;

	// 총알의 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	float Damage = 5.0f;

	// 총알이 터질때 사용할 이팩트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	UNiagaraSystem* ExplosionEffect = nullptr;

	// 총알의 초기 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	float InitialSpeed = 500.0f;
	
	// 총알의 색상
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	FLinearColor ProjectileColor = FLinearColor::White;

	// 수명 타이머의 핸들러
	FTimerHandle LifeTimeTimerHandle;
};
