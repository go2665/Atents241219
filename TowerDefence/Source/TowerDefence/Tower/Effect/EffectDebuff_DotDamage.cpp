// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_DotDamage.h"
#include "Kismet/GameplayStatics.h"

void UEffectDebuff_DotDamage::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, int32 InLevel, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InLevel, InTarget);

	//InData->Modifier1;	// 총 데미지
	Interval = GetModifier2();	// 인터벌
	BaseTickDamage = GetModifier1() / (GetDuration() / Interval);	// 틱당 데미지 결정
	TickDamage = BaseTickDamage;	// 틱당 데미지 적용
}

void UEffectDebuff_DotDamage::OnBegin()
{
	Super::OnBegin();

	// 타이머 시작
	UWorld* World = GetWorld();	
	if (World)
	{
		World->GetTimerManager().SetTimer(
			TimerHandle, 
			this, &UEffectDebuff_DotDamage::DealTarget, 
			Interval, true);
	}
}

void UEffectDebuff_DotDamage::OnEnd()
{
	Super::OnEnd();

	// 타이머 종료
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UEffectDebuff_DotDamage::DealTarget()
{
	UGameplayStatics::ApplyDamage(
		Target,
		TickDamage,
		nullptr,
		nullptr,
		DamageTypeClass);
}
