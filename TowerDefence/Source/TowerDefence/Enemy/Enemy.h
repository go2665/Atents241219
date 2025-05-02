// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Enemy/Data/EnemyDataAsset.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/Components/Effect/EffectTargetable.h"
#include "TowerDefence/Components/Effect/EffectComponent.h"
#include "Enemy.generated.h"

UCLASS()
class TOWERDEFENCE_API AEnemy : public AActor, public IEffectTargetable
{
	GENERATED_BODY()
	
public:	
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; // 데미지 처리 함수

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool AddEffect(EEffectType InType, int32 InLevel) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool RemoveEffect(EEffectType InType) override;

	void ApplyModifiers(const TMap<EEffectModifier, float>* InModifierMap) override;
		
	//inline virtual void SetHealth(float InHealth) override;
	//inline virtual bool IsAlive() const override;

	// 모디파이어 맵을 가져오는 함수
	inline virtual const TMap<EEffectModifier, float>* GetModifiersMap() const override { return EffectModifiers; }

	// 특정 모디파이어를 가져오는 함수
	inline virtual float GetModifier(EEffectModifier ModifierType) const override
	{
		if (EffectModifiers && EffectModifiers->Contains(ModifierType))
		{
			return (*EffectModifiers)[ModifierType];
		}
		return 0.0f;
	}

	inline virtual EEffectTarget GetEffectTarget() const override { return EEffectTarget::Hostile; }

private:
	// 적의 체력 설정
	void SetHealth(float InHealth);

	// 이 적의 데이터값 가져오기
	inline const UEnemyDataAsset* GetEnemyData() {
		return EnemyData;
	};

protected:
	// 적의 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMeshComponent* EnemyMesh = nullptr;			
	
	// 적의 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UEnemyDataAsset* EnemyData = nullptr;	

	// 적의 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 20.0f;

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

	// 모디파이어 맵의 주소(EffectComponent가 전달해준다)
	const TMap<EEffectModifier, float>* EffectModifiers = nullptr;

};
