// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_HealthBar.h"
#include "Components/ProgressBar.h"
#include "Action/Player/ActionPlayerCharacter.h"

void UUserWidget_HealthBar::SetHealth(float InHealth, float InMaxHealth)
{
	MaxHealth = InMaxHealth;
	OnHealthChange(InHealth);
}

void UUserWidget_HealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	AActionPlayerCharacter* PlayerCharacter = Cast<AActionPlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnHealthChange.AddDynamic(this, &UUserWidget_HealthBar::OnHealthChange);

		SetHealth(PlayerCharacter->GetCurrentHealth(), PlayerCharacter->GetMaxHealth());
	}
}

void UUserWidget_HealthBar::OnHealthChange(float InHealth)
{
	if (Progress != nullptr)
	{
		Progress->SetPercent(InHealth / MaxHealth);
	}
}
