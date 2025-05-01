// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBase.h"

void UEffectBase::Tick(float DeltaTime)
{
	RemainingDuration -= DeltaTime;
	if (RemainingDuration > 0.0f)
	{
		OnTick(DeltaTime); // 이팩트별 틱 처리
	}
	else
	{
		OnEnd(); // 이팩트 종료
		OnEffectExpire.ExecuteIfBound(this); // 이팩트 만료 델리게이트 호출
	}
}

void UEffectBase::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, int32 InEffectLevel, AActor* InTarget)
{
	EffectType = InType;
	EffectDataAsset = InData;
	EffectLevel = FMath::Min(InEffectLevel, EffectDataAsset->LevelData.Num() - 1);
	Target = InTarget;
	StackCount = 1;

	UpdateModifiers();
}

void UEffectBase::OnBegin()
{
	// 버프가 시작 되었을 때 할일
	RemainingDuration = GetDuration();
	// 이 이팩트가 처음 적용될 때 일어나야 하는 로직을 여기에 추가합니다.
}

void UEffectBase::OnTick(float DeltaTime)
{
	// 이팩트별 개별 탁 적용
}

void UEffectBase::OnEnd()
{
	// 이 이팩트가 끝났을 때 원래대로 되돌리는 로직을 여기에 추가합니다.
	RemainingDuration = 0.0f;
	EffectModifiers.Empty();
}

void UEffectBase::OnStack(int InLevel)
{
	EffectLevel = FMath::Min(InLevel, EffectDataAsset->LevelData.Num() - 1);
	RemainingDuration = GetDuration(); // 이팩트의 지속 시간 초기화
	StackCount = FMath::Min(StackCount + 1, GetMaxStackCount()); // 스택 수 증가 + 최대 스택 수 제한

	UpdateModifiers();
}

void UEffectBase::OnExtend(int InLevel)
{
	EffectLevel = FMath::Min(InLevel, EffectDataAsset->LevelData.Num() - 1);
	RemainingDuration += GetDuration(); // 이팩트의 지속 시간 연장
}
