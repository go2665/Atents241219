// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff_DotDamage.h"
#include "TowerDefence/Enemy/DEPRECATED_Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

UDebuff_DotDamage::UDebuff_DotDamage()
{
	DebuffName = FName("DotDamage");
	DebuffType = EDebuffType::DotDamage;
	DefaultDuration = 3.0f;
	CurrentDuration = 0.0f;
}

void UDebuff_DotDamage::OnInitialize(class AEnemyBase* InTarget, float InModifier)
{
	Super::OnInitialize(InTarget, InModifier);

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(
		TimerHandle, 
		this, &UDebuff_DotDamage::OnDotDamageTick, 
		DotInterval, true, 0); // 타이머 설정
	
}

void UDebuff_DotDamage::OnEnd()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle); // 타이머 해제

	Super::OnEnd();
}

void UDebuff_DotDamage::OnDotDamageTick()
{
	UGameplayStatics::ApplyDamage(
		TargetEnemy,
		DotDamage * ModifierValue,
		nullptr,
		nullptr,
		nullptr); // 지속 피해 적용
}
