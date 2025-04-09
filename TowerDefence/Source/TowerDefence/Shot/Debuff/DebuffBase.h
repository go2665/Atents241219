// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebuffBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOWERDEFENCE_API UDebuffBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnAdd();		// 디버프가 추가 되었을 때 할일
	virtual void OnApply();		// 개별 디버프 적용

	inline float GetDuration() const { return Duration; } // 디버프 지속 시간 반환
	inline void DecreaseDuration(float Decrease) { Duration -= Decrease; } // 디버프 지속 시간 감소

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float Duration = 1.0f; // 디버프 지속 시간

};
