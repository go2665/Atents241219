// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingProjectileBase.h"
#include "PoolingProjectile_Area.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingProjectile_Area : public APoolingProjectileBase
{
	GENERATED_BODY()
	
public:
	APoolingProjectile_Area();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	virtual void OnExplosion() override;

private:
	// 2차 폭발 시작
	void SecondExplosion();

	// 2차 폭발 범위에 있는 모든 대상에게 데미지 적용
	void ApplyDamageToSecondTargets();

	// 2차 폭발 범위에 들어오면 실행될 함수(대상 추가)
	UFUNCTION()
	void OnSecondAreaBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 2차 폭발 범위에서 나가면 실행될 함수(대상 제거)
	UFUNCTION()
	void OnSecondAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

protected:
	// 2차 폭발 범위 표시용 컬리젼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	USphereComponent* AreaCollision = nullptr;

	// 2차 폭발 이펙트용 비주얼 이펙트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	UNiagaraComponent* AreaVfx = nullptr;

	// 2차 폭발 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	float SecondDuration = 3.0f;

	// 2차 폭발 데미지 틱 간격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	float SecondTickInterval = 0.1f;

private:
	// 2차 폭발 데미지 타겟들
	UPROPERTY()
	TArray<AActor*> SecondTargets;

	UPROPERTY()
	FTimerHandle SecondExplosionTimerHandle;
};
