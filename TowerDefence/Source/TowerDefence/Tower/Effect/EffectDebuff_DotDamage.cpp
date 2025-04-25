// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_DotDamage.h"
#include "Kismet/GameplayStatics.h"

void UEffectDebuff_DotDamage::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);

	//InData->Modifier1;	// 총 데미지
	Interval = InData->Modifier2;	// 인터벌
	BaseTickDamage = InData->Modifier1 / (InData->Duration / Interval);	// 틱당 데미지 결정
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

void UEffectDebuff_DotDamage::OnStack()
{
	Super::OnStack();

	TickDamage = BaseTickDamage * StackCount;	// 스택 수에 따라 틱당 데미지 증가
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
