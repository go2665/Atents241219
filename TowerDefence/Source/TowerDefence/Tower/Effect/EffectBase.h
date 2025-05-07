// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Tower/Data/EffectDataAsset.h"
#include "EffectBase.generated.h"

class ATowerBaseActor;

// 이팩트 만료 델리게이트(이팩트가 만료 되었을 때 호출됨)
DECLARE_DELEGATE_OneParam(FOnEffectExpire, UEffectBase*);	

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UEffectBase : public UObject
{
	GENERATED_BODY()

public:
	// 컴포넌트의 틱이 호출될 때마다 호출 될 함수
	void Tick(float DeltaTime);

	// 이팩트가 생성 되었을 때 할일
	virtual void OnInitialize(
		EEffectType InType, const UEffectDataAsset* InData, int32 InEffectLevel, AActor* InTarget);

	// 이팩트가 시작 되었을 때 할일
	virtual void OnBegin();						

	// 이팩트별로 틱에서 추가 작업을 수행하는 함수
	virtual void OnTick(float DeltaTime);	

	// 이팩트가 끝났을 때 할일
	virtual void OnEnd();	

	// 이팩트 타입 반환
	inline EEffectType GetEffectType() const { return EffectType; }

	// 이팩트 지속 시간 반환
	inline float GetCurrentDuration() const { return RemainingDuration; }				

	// 파생 클래스에서 필요한 경우에만 재정의
	inline float GetModifierValue(EEffectModifier InType) const
	{
		if (EffectModifiers.Contains(InType))
			return EffectModifiers[InType];
		return 0.0f;	// 모디파이어는 0을 기준으로 +-되는 배율이기 때문에 없으면 0을 리턴
	}

protected:
	// 모디파이어 업데이트 하는 함수(초기화 할 때와 스택 될 때 실행)
	virtual void UpdateModifiers() {}

	// 이팩트의 현재 지속 시간
	inline float GetDuration() const { return EffectDataAsset->LevelData[EffectLevel].Duration; }	
	
	// 이팩트의 최대 스택 수
	inline int32 GetMaxStackCount() const { return EffectDataAsset->LevelData[EffectLevel].MaxStackCount; }
	
	// 이팩트의 모디파이어1
	inline float GetModifier1() const { return EffectDataAsset->LevelData[EffectLevel].Modifier1; }

	// 이팩트의 모디파이어2
	inline float GetModifier2() const { return EffectDataAsset->LevelData[EffectLevel].Modifier2; }

	// 이팩트의 모디파이어3
	inline float GetModifier3() const { return EffectDataAsset->LevelData[EffectLevel].Modifier3; }
	

public:
	// 이팩트 만료 델리게이트(이팩트가 만료 되었을 때 호출됨)
	FOnEffectExpire OnEffectExpire;	

protected:
	// 이팩트 타입
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
	EEffectType EffectType = EEffectType::None;

	// 이팩트 데이터 에셋(필수)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
	const UEffectDataAsset* EffectDataAsset = nullptr;	

	// 이 이팩트가 적용된 대상
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
	AActor* Target = nullptr; 

	// 이팩트 지속 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
	float RemainingDuration = 0.0f; 

	// 이팩트 수치(타입, 수치) (생성자에서 적용할 모디파이어만 타입별로 추가)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
	TMap<EEffectModifier, float> EffectModifiers;	

	// 이팩트 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 EffectLevel = 0;	// (0에서 시작)	
};
