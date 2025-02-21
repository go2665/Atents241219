// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWing.h"
#include "ObjectPoolActorComponent.h"
#include "NiagaraSystem.h"
#include "PlayerWingUsePool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDie);

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APlayerWingUsePool : public APlayerWing
{
	GENERATED_BODY()
	
public:
	APlayerWingUsePool();							// 풀 컴포넌트 초기화용
	virtual void Tick(float DeltaTime) override;	// 발사체 쿨타임 관리용

	inline const UInputMappingContext* GetMappingContext() const {
		return DefaultMappingContext;
	};
	inline bool IsAlive() const { return Health >= 0; }
	inline float GetHealth() const { return Health; }
	inline float GetMaxHealth() const { return MaxHealth; }

protected:
	virtual void BeginPlay() override;				// 데미지 받는 함수 바인딩용
	virtual void OnFireStart(EProjectileType Type) override;	// 발사체 발사 시작(연사 처리용)
	virtual void OnFireEnd(EProjectileType Type) override;		// 발사체 발사 종료(연사 처리용)

	// Normal 발사체 발사 트랜스폼 받아오는 함수
	const FTransform GetFireTransform() const;		

	// 이런식으로 만드는 것도 가능(가독성이 더 좋음)
	inline bool IsFireReady_Normal() const { return FireCoolTime_Normal < 0; }
	inline bool IsFireReady_Homing() const { return FireCoolTime_Homing < 0; }
	inline bool IsFireReady_Area() const { return FireCoolTime_Area < 0; }	

private:
	// 데미지 받는 함수
	UFUNCTION()
	void OnPlayerTakeAnyDamage(
		AActor* DamagedActor, float Damage, 
		const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// 사망 처리 함수
	void DieProcess();	

public:
	// 사망 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Object Pool")
	FOnPlayerDie OnDie;

protected:
	// Normal 발사체 풀 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Object Pool")
	UObjectPoolActorComponent* NormalProjectilePool = nullptr;

	// Homing 발사체 풀 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Object Pool")
	UObjectPoolActorComponent* HomingProjectilePool = nullptr;

	// Area 발사체 풀 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Object Pool")
	UObjectPoolActorComponent* AreaProjectilePool = nullptr;

	// 연사시 발사 간격(Normal)
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Interval_Normal = 0.5f;

	// 연사시 발사 간격(Homing)
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Interval_Homing = 1.0f;

	// 연사시 발사 간격(Area)
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Interval_Area = 1.0f;

	// 연사용 타이머 핸들(Normal)
	UPROPERTY()
	FTimerHandle TimerHandle_Normal;

	// 연사용 타이머 핸들(Homing)
	UPROPERTY()
	FTimerHandle TimerHandle_Homing;

	// 연사용 타이머 핸들(Area)
	UPROPERTY()
	FTimerHandle TimerHandle_Area;

	// 발사체 쿨타임(Normal)
	float FireCoolTime_Normal = Interval_Normal;

	// 발사체 쿨타임(Homing)
	float FireCoolTime_Homing = Interval_Homing;

	// 발사체 쿨타임(Area)
	float FireCoolTime_Area = Interval_Area;
	
	// 플레이어 최대 HP
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float MaxHealth = 100.0f;

	// 플레이어 HP
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float Health = MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	UNiagaraSystem* DieExplosion = nullptr;


	//mutable bool test = false; // mutable : const 함수 내에서 변수를 변경할 수 있도록 함.
};
