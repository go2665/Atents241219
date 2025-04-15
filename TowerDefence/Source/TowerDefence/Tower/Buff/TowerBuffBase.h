// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TowerBuffEnums.h"
#include "TowerBuffDataAsset.h"
#include "TowerBuffBase.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuffBase : public UObject
{
	GENERATED_BODY()

public:
	// 버프가 생성 되었을 때 할일
	virtual void OnInitialize(UTowerBuffDataAsset* Data);
	virtual void OnBuffBegin();						// 버프가 시작 되었을 때 할일
	virtual void OnBuffTick(float DeltaTime);		// 버프 틱별 개별 적용
	virtual void OnBuffEnd();						// 버프가 끝났을 때 할일

	inline ETowerBuffType GetBuffType() const { return BuffDataAsset->BuffType; }	// 버프 타입 반환
	inline float GetCurrentDuration() const { return CurrentDuration; }				// 버프 지속 시간 반환
	inline void ExtendDuration() { CurrentDuration += BuffDataAsset->Duration; }	// 버프 지속 시간 추가
	inline void ResetDuration() { CurrentDuration = BuffDataAsset->Duration; }		// 버프 지속 시간 초기화

	// 파생 클래스에서 필요한 경우에만 재정의
	inline float GetModifierValue(ETowerBuffModifier Type) const
	{
		if (BuffModifiers.Contains(Type))
			return BuffModifiers[Type];
		return 1.0f;	// 모디파이어는 전부 배율이기 때문에 없으면 1을 리턴
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	UTowerBuffDataAsset* BuffDataAsset = nullptr;	// 버프 데이터 에셋(필수)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float CurrentDuration = 0.0f; // 디버프 지속 시간

	UPROPERTY()
	TMap<ETowerBuffModifier, float> BuffModifiers;	// 버프 수치(타입, 수치) (생성자에서 적용할 모디파이어만 타입별로 추가)
};
