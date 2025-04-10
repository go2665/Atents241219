// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebuffEnums.h"
#include "DebuffBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOWERDEFENCE_API UDebuffBase : public UObject
{
	GENERATED_BODY()

public:
	UDebuffBase();

	virtual void OnInitialize(class AEnemyBase* Target);	// 디버프가 추가 되었을 때 할일
	virtual void OnTick(float DeltaTime);					// 개별 디버프 적용
	virtual void OnEnd();									// 디버프가 끝났을 때 할일

	inline EDebuffType GetDebuffType() const { return DebuffType; }				// 디버프 타입 반환
	inline float GetCurrentDuration() const { return CurrentDuration; }			// 디버프 지속 시간 반환
	inline void ExtendDuration() { CurrentDuration += DefaultDuration; }		// 디버프 지속 시간 추가

	// 파생 클래스에서 필요한 경우에만 재정의
	inline virtual float GetSpeedModifier() const { return 0.0f; }			
	inline virtual float GetDamageModifier() const { return 0.0f; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	EDebuffType DebuffType = EDebuffType::None; // 디버프 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	FName DebuffName; // 디버프 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float CurrentDuration = 0.0f; // 디버프 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DefaultDuration = 1.0f; // 디버프 기본 지속 시간

	UPROPERTY()
	class AEnemyBase* TargetEnemy = nullptr; // 이 디버프가 적용된 적

};
