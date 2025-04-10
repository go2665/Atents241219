// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Shot/Data/ShotEnums.h"
#include "TowerDefence/Shot/Attribute/TowerDamageType.h"
#include "TowerDefence/Shot/Debuff/DebuffEnums.h"

#include "EnemyBase.generated.h"

UCLASS()
class TOWERDEFENCE_API AEnemyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; // 데미지 처리 함수

public:
	void RestorSpeedeModifier(EDebuffType RemoveType);
	void RestorDamageModifier(EDebuffType RemoveType);

	inline void SetSpeedModifier(float NewSpeedModifier) { SpeedModifier = NewSpeedModifier; };	// 이동 속도 수정 값 설정
	inline void SetDamageModifier(float NewDamageModifier) { DamageModifier = NewDamageModifier; };	// 데미지 배율 설정

private:
	void SetCurrentHealth(float NewHealth);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMeshComponent* EnemyMesh = nullptr;			// 적의 메쉬 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDebuffComponent* DebuffComponent = nullptr;		// 적의 디버프 관리 컴포넌트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<UTowerDamageType> WeakType = nullptr;	// 적의 약점 속성

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 20.0f;						// 적의 현재 체력

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float Speed = 100.0f;								// 적의 이동 속도

private:
	float SpeedModifier = 0.0f;							// 적의 이동 속도 수정 값
	float DamageModifier = 1.0f;						// 적의 데미지 배율
};
