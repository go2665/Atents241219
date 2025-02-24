// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingObjectBase.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EDamageType.h"
#include "NiagaraSystem.h"	
#include "PoolingEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemyBase : public APoolingObjectBase
{
	GENERATED_BODY()
	
public:
	APoolingEnemyBase();

protected:
	virtual void BeginPlay() override;	
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	// 플레이어를 공격할 때 사용하는 함수(주기적으로 실행)
	virtual void Attack(AActor* Target);

	// 사망 처리 함수
	virtual void Die();

	// 생존 여부 확인 함수
	inline bool IsAlive() const { return Health >= 0.0f; }

private:
	// 데미지를 입었을 때 호출되는 함수
	UFUNCTION()
	void OnEnemyAnyDamage(
		AActor* DamagedActor, 
		float Damage, 
		const UDamageType* DamageType, 
		AController* InstigatedBy, 
		AActor* DamageCauser);

	// 다른 액터와 겹칠 때 호출되는 함수
	UFUNCTION()
	void OnEnemyActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// 다른 액터가 나갔을 때 호출되는 함수
	UFUNCTION()
	void OnEnemyActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// 적이 받는 최종 데미지를 계산하는 함수
	float CalcFinalDamage(float BaseDamage, const UDamageType* DamageType);

protected:
	// 적의 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UStaticMeshComponent* StaticMesh = nullptr;

	// 앞쪽을 표시하기 위한 화살표 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UArrowComponent* Arrow = nullptr;

	// 적의 이동을 처리할 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float MoveSpeed = 100.0f;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float MaxHealth = 100.0f;

	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Health = 100.0f;

	// 약점 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	EDamageType WeakType = EDamageType::DT_Red;

	// 약점 속성에 대한 데미지 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float WeakTypeDamageModifier = 2.0f;

	// 자신이 터질 때 재생될 파티클 시스템
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UNiagaraSystem* SelfExplosionEffect = nullptr;

	// 적의 수명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float LifeTime = 5.0f;

	// 초당 적이 주는 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float DamagePerSecond = 10.0f;

	// 데미지를 입히는 주기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float DamageTickInterval = 0.05f;

	// 플레이어에게 주는 점수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	int32 Point = 10;

	// 초기화 되었는지 여부(생성 후 첫 비활성화때 이팩트 재생 또는 소형 운석 생성 방지용)
	bool bIsInitialized = false;

private:	
	// 적의 수명 타이머
	UPROPERTY()
	FTimerHandle LifeTimeTimer;

	// 플레이어에게 데미지를 주기적으로 주기 위한 타이머
	UPROPERTY()
	FTimerHandle DamageTimer;

	UPROPERTY()
	class ACppGameState* GameState = nullptr;
};
