// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset_Food.h"
#include "../../Player/ActionPlayerCharacter.h"

void UItemDataAsset_Food::UseItem(AActor* Target)
{
	// Target에게 음식 사용(Target은 항상 플레이어일때만 실행)
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,TEXT("Use Food"));
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(Target);
	if (Player)
	{
		Player->RestoreHealthPerTick(HealAmount, TickInterval, TickCount);
	}
}
