// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffComponent.h"
#include "Debuff_Slow.h"
#include "Debuff_Stun.h"
#include "Debuff_DotDamage.h"
#include "TowerDefence/Enemy/EnemyBase.h"

UDebuffComponent::UDebuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDebuffComponent::BeginPlay()
{
	Super::BeginPlay();

	ActiveDebuffs.Empty(); // Initialize the ActiveDebuffs array	
	RemoveIndices.Reserve(static_cast<uint8>(EDebuffType::COUNT));
}

void UDebuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RemoveIndices.Empty(); 	
	for (int32 i = 0; i < ActiveDebuffs.Num(); i++)
	{
		ActiveDebuffs[i]->OnTick(DeltaTime);
		if (ActiveDebuffs[i]->GetCurrentDuration() <= 0.0f)
		{
			ActiveDebuffs[i]->OnEnd();
			RemoveIndices.Add(i);
		}
	}

	for (int32 i = RemoveIndices.Num() - 1; i >= 0 ; i--)
	{
		ActiveDebuffs.RemoveAt(RemoveIndices[i]);
	}
}

void UDebuffComponent::AddDebuff(EDebuffType Type)
{
	UDebuffBase* NewDebuff = CreateDebuff(Type);
	if (NewDebuff)
	{
		NewDebuff->OnInitialize(Cast<AEnemyBase>(GetOwner()));
		ActiveDebuffs.Add(NewDebuff);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create debuff of type: %s"), *UEnum::GetValueAsString(Type));
	}
}

void UDebuffComponent::RemoveDebuff(EDebuffType Type)
{
	for (int32 i = 0; i < ActiveDebuffs.Num(); i++)
	{
		if (ActiveDebuffs[i]->GetDebuffType() == Type)
		{
			ActiveDebuffs[i]->OnEnd();
			ActiveDebuffs.RemoveAt(i);
			break;
		}
	}
}

float UDebuffComponent::GetMaxiumModifierValue(EDebuffType IgnoreType, EDebuffModifier ModifierType)
{
	// 모든 모디파이어는 숫자가 클수록 효과가 크다.
	// IgnoreType를 제외하고 가장 효과가 큰 모디파이어를 찾는다.
	float MaxModifier = 0.0f;

	for (auto& Debuff : ActiveDebuffs)
	{
		if (Debuff->GetDebuffType() == IgnoreType)
			continue;
		switch (ModifierType)
		{
		case EDebuffModifier::Speed:
			MaxModifier = FMath::Max(MaxModifier, Debuff->GetSpeedModifier());
			break;
		case EDebuffModifier::Damage:
			MaxModifier = FMath::Max(MaxModifier, Debuff->GetDamageModifier());
			break;
		default:
			break;
		}
	}

	return MaxModifier;
}

UDebuffBase* UDebuffComponent::CreateDebuff(EDebuffType Type)
{
	UDebuffBase* NewDebuff = nullptr;
	switch (Type)
	{
	case EDebuffType::Slow:
		NewDebuff = NewObject<UDebuff_Slow>(this, UDebuff_Slow::StaticClass(), TEXT("SlowDebuff"));
		break;
	case EDebuffType::Stun:
		NewDebuff = NewObject<UDebuff_Stun>(this, UDebuff_Stun::StaticClass(), TEXT("StunDebuff"));
		break;
	case EDebuffType::DotDamage:
		NewDebuff = NewObject<UDebuff_DotDamage>(this, UDebuff_DotDamage::StaticClass(), TEXT("DotDamageDebuff"));
		break;
		// Add more cases for other debuff types as needed
	default:
		break;
	}

	return NewDebuff;
}

