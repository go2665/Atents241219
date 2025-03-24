// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ANormalEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	ANormalEnemy();

	UFUNCTION(BlueprintCallable)
	void Attack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxWalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	class UAnimMontage* AttackMontage = nullptr;
};

// 적의 공격이 플레이어에게 데미지를 입히도록 구현
//	1. 데미지량은 적의 무기에 설정
//  2. AnimNotifyState를 사용하여 무기의 충돌을 활성화/비활성화