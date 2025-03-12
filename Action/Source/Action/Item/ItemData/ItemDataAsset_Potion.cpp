// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset_Potion.h"
#include "../../Player/ActionPlayerCharacter.h"

void UItemDataAsset_Potion::UseItem(AActor* Target)
{
	// Target에게 포션 사용(Target은 항상 플레이어일때만 실행)
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(Target);
	if (Player)
	{
		Player->RestoreHealth(Player->GetMaxHealth() * HealAmount);
	}

}
