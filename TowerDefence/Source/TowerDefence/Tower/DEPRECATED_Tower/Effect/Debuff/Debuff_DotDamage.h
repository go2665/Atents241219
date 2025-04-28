// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebuffBase.h"
#include "Debuff_DotDamage.generated.h"

/**
 *	일정 시간 간격으로 데미지를 주는 디버프 
 */
UCLASS()
class TOWERDEFENCE_API UDebuff_DotDamage : public UDebuffBase
{
	GENERATED_BODY()
	
public:
	UDebuff_DotDamage();
	virtual void OnInitialize(class AEnemyBase* InTarget, float InModifier = 1.0f) override;
	virtual void OnEnd() override;

private:
	UFUNCTION()
	void OnDotDamageTick();	// 지속 피해 주는 함수

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DotDamage = 1.0f;		// 지속 피해량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DotInterval = 0.2f;	// 지속 피해 주는 간격

private:
	FTimerHandle TimerHandle;
};
