// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebuffBase.h"
#include "Debuff_Slow.generated.h"

/**
 * 이동 속도를 감소시키는 디버프 클래스
 */
UCLASS()
class TOWERDEFENCE_API UDebuff_Slow : public UDebuffBase
{
	GENERATED_BODY()

public:
	UDebuff_Slow();
	virtual void OnInitialize(class AEnemyBase* Target) override;	
	virtual void OnEnd() override;	

	inline virtual float GetSpeedModifier() const override { return SlowRate; }	// 이동 속도 비율 반환

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float SlowRate = 0.3f;	// 이동 속도 감소 비율
};
