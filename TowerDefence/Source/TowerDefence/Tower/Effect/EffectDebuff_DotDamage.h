// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBase.h"
#include "EffectDebuff_DotDamage.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UEffectDebuff_DotDamage : public UEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialize(EEffectType InType, const UEffectDataAsset* InData, int32 InLevel, AActor* InTarget) override;
	virtual void OnBegin() override;
	virtual void OnEnd() override;

	// 데미지 타입을 설정합니다.
	inline void SetDamageType(TSubclassOf<UDamageType> InDamageType) { DamageTypeClass = InDamageType; }


private:
	UFUNCTION()
	void DealTarget();

private:
	// 데미지 인터벌
	float Interval = 0.0f;		

	// 틱당 데미지 양
	float TickDamage = 0.0f;	

	// 기본 틱당 데미지 양(초기화 때 결정)
	float BaseTickDamage = 0.0f;

	// 타이머 핸들
	FTimerHandle TimerHandle;	

	// 데미지 타입
	TSubclassOf<UDamageType> DamageTypeClass = nullptr;	
};
