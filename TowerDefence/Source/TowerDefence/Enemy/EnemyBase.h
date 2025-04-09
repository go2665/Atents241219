// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Shot/Data/ShotEnums.h"
#include "TowerDefence/Shot/Attribute/TowerDamageType.h"
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

private:
	void SetCurrentHealth(float NewHealth);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMeshComponent* EnemyMesh = nullptr;			// 적의 메쉬 컴포넌트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<UTowerDamageType> WeakType = nullptr;	// 적의 약점 속성

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 20.0f;						// 적의 현재 체력

};
