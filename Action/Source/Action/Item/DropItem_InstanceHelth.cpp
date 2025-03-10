// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_InstanceHelth.h"
#include "../Player/ActionPlayerCharacter.h"
#include "ItemDataAsset_Heal.h"

void ADropItem_InstanceHelth::OnGetItem(AActor* Target)
{
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(Target);
	UItemDataAsset_Heal* HealData = Cast<UItemDataAsset_Heal>(ItemDataAsset);
	if (Player && HealData)
	{
		Player->RestoreHealth(HealData->HealAmount, HealData->HealDuration);
	}
}
