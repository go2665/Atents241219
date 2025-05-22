// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Enemy/Data/EnemyDataAsset.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/Components/Effect/EffectTargetable.h"
#include "TowerDefence/Components/Effect/EffectComponent.h"
#include "TowerDefence/Framework/ObjectPool/PoolableActor.h"
#include "Enemy.generated.h"

class USplineComponent;
class UMeshComponent;
class UNiagaraSystem;


// 타워가 적을 죽였을 때 호출되는 델리게이트(파라메터는 플레이어가 획득할 골드)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, int32); 

// 적이 골인 지점에 도착해서 플레이어에게 데미지를 줄 때 호출되는 델리게이트(파라메터는 적이 주는 데미지)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyAttack, int32);

UCLASS()
class TOWERDEFENCE_API AEnemy : public AActor, public IEffectTargetable, public IPoolableActor
{
	GENERATED_BODY()
	
public:	
	AEnemy();

protected:

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; // 데미지 처리 함수

	// 스폰되었을 때 적을 초기화하는 함수
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void OnSpawn(USplineComponent* InSpline, const FVector& InOffset);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool AddEffect(EEffectType InType, int32 InLevel) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool RemoveEffect(EEffectType InType) override;

	void ApplyModifiers(const TMap<EEffectModifier, float>* InModifierMap) override;

	void Die(bool bGoalArrived);

	// IPoolableActor을(를) 통해 상속됨 ---------------------------------
	void OnInitialize() override;
	void OnActivate() override;
	void OnDeactivate() override;
	inline EPooledActorType GetPoolType() const override { return PoolType; };
	inline void SetPoolType(EPooledActorType InType) override { PoolType = InType; };
	// -----------------------------------------------------------------
		
	//inline virtual void SetHealth(float InHealth) override;
	//inline virtual bool IsAlive() const override;

	// 모디파이어 맵을 가져오는 함수
	inline const TMap<EEffectModifier, float>* GetModifiersMap() const override { return EffectModifiers; }

	// 특정 모디파이어를 가져오는 함수
	inline float GetModifier(EEffectModifier ModifierType) const override
	{
		if (EffectModifiers && EffectModifiers->Contains(ModifierType))
		{
			return (*EffectModifiers)[ModifierType];
		}
		return 0.0f;
	}

	// 이팩트를 받는 아군인지 적군인지 알려주는 함수
	inline virtual EEffectTarget GetEffectTarget() const override { return EEffectTarget::Hostile; }

	// 이팩트의 특수효과가 보일 위치를 가져오는 함수
	inline FVector GetEffectLocation() const override
	{
		if (EnemyMesh)
		{
			return EnemyMesh->GetSocketLocation(FName("EffectTarget"));
		}
		else
		{
			return GetActorLocation();
		}
	};

	inline int32 GetDamage() const
	{
		return EnemyData ? EnemyData->Damage : 0;
	}

	inline bool IsAlive() const
	{
		return bIsAlive;
	}

private:
	// 적의 체력 설정
	void SetHealth(float InHealth);

	// 이 적의 데이터값 가져오기
	inline const UEnemyDataAsset* GetEnemyData() {
		return EnemyData;
	};

public:
	FOnEnemyKilled OnEnemyKilled;	// 적이 죽었을 때 호출되는 델리게이트
	FOnEnemyAttack OnEnemyAttack;	// 적이 골인 지점에 도착했을 때 호출되는 델리게이트

protected:
	// 적의 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMeshComponent* EnemyMesh = nullptr;			
	
	// 적의 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UEnemyDataAsset* EnemyData = nullptr;	

	// 적의 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 20.0f;

	// 이펙트의 위치 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FVector HitEffectOffset = FVector(0.0f, 0.0f, 100.0f);

	// 히트 이펙트 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UNiagaraSystem* HitEffectAsset = nullptr; 

	// 적의 이동 속도(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Effect Modified Value")
	float Speed = 100.0f;						

	// 방어력(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Effect Modified Value")
	float Defence = 0.0f;	

	// 디버그 정보 표시 여부
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debug")
	bool bShowDebugInfo = false;

private:
	// 이팩트 컴포넌트 인스턴스
	UPROPERTY()
	UEffectComponent* EffectComponent = nullptr;

	// 스포너의 스플라인 컴포넌트
	USplineComponent* SpawnerSpline = nullptr;

	// 모디파이어 맵의 주소(EffectComponent가 전달해준다)
	const TMap<EEffectModifier, float>* EffectModifiers = nullptr;

	// 현재 스플라인을 따라 이동한 거리
	float CurrentDistance = 0.0f;

	// 위치 오프셋
	FVector Offset = FVector::ZeroVector;

	// 적이 살아있는지 여부
	bool bIsAlive = true;	

	// 풀 액터 타입(적 캐릭터 타입중 하나 선택)
	EPooledActorType PoolType = EPooledActorType::EnemySpeed;	
};
