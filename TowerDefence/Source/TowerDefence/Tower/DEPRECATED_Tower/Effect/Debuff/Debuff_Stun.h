// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebuffBase.h"
#include "Debuff_Stun.generated.h"

/**
 * 적을 정지 시키고 추가 데미지를 받게 만드는 디버프 클래스
 */
UCLASS()
class TOWERDEFENCE_API UDebuff_Stun : public UDebuffBase
{
	GENERATED_BODY()

public:
	UDebuff_Stun();
	virtual void OnInitialize(class AEnemyBase* InTarget, float InModifier = 1.0f) override;
	virtual void OnEnd() override;

	inline virtual float GetSpeedModifier() const override { return 1 - StunSpeedModifier; }
	inline virtual float GetDamageModifier() const override { return StunDamageModifier; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float StunSpeedModifier = 0.0f;	// 스턴 속도 감소 비율

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float StunDamageModifier = 0.5f;	// 스턴 추가 데미지 비율
	
};
