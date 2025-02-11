// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingProjectileBase.h"
#include "PoolingProjectile_Homing.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingProjectile_Homing : public APoolingProjectileBase
{
	GENERATED_BODY()
public:
	APoolingProjectile_Homing();
	
protected:
	// BeginPlay 오버라이딩(호밍 오버랩 함수 바인딩용)
	virtual void BeginPlay() override;

	// 호밍 발사체가 비활성화될 때 호출되는 함수(호밍 관련 데이터 리셋 용도)
	virtual void OnDeactivate() override;

private:
	// HomingRadius에 다른 액터가 겹칠 때 호출되는 함수
	UFUNCTION()
	void OnHomingBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//UFUNCTION()
	//void HomingReset();

protected:
	// 추적 반경을 설정하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	USphereComponent* HomingRadius = nullptr;

	// 추적 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooled Projectile")
	float HomingDuration = 3.0f;

private:
	FTimerHandle HomingResetTimerHandle;
};
