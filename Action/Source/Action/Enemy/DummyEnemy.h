// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "DummyEnemy.generated.h"


UCLASS()
class ACTION_API ADummyEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 데미지 입었을 때 호출
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	// 사망시 호출
	virtual void Die() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:


protected:


private:
	

};
