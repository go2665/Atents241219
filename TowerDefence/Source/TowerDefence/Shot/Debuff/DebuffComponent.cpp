// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffComponent.h"
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

	for (int32 i = 0; i < RemoveIndices.Num(); i++)
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

UDebuffBase* UDebuffComponent::CreateDebuff(EDebuffType Type)
{
	UDebuffBase* NewDebuff = nullptr;
	switch (Type)
	{
	case EDebuffType::Slow:
		NewDebuff = NewObject<UDebuffBase>(this, UDebuffBase::StaticClass(), TEXT("SlowDebuff"));
		break;
	case EDebuffType::Stun:
		NewDebuff = NewObject<UDebuffBase>(this, UDebuffBase::StaticClass(), TEXT("StunDebuff"));
		break;
	case EDebuffType::DotDamage:
		NewDebuff = NewObject<UDebuffBase>(this, UDebuffBase::StaticClass(), TEXT("DotDamageDebuff"));
		break;
		// Add more cases for other debuff types as needed
	default:
		break;
	}

	return NewDebuff;
}

