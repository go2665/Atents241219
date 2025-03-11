// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

UCLASS()
class ACTION_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die();
	void DropItems();

public:
	FOnDie OnDie;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	float MaxHealth = 10.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 10.0f;

	// 드랍 아이템 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	class UDataTable* DropItemDataTable = nullptr;

	// 아이템을 얼마나 멀리 던질지 결정하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	float ItemLaunchPower = 500.0f;

private:
	bool bIsAlive = true;

};
