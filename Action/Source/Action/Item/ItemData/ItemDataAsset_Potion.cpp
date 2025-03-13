// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset_Potion.h"
#include "../../Player/ActionPlayerCharacter.h"

void UItemDataAsset_Potion::UseItem(AActor* Target)
{
	// Target에게 포션 사용(Target은 항상 플레이어일때만 실행)
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
	//	FString::Printf(TEXT("Use Potion"), HealAmount));
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(Target);
	if (Player)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
		//	FString::Printf(TEXT("HealAmount : %.1f"), HealAmount));

		Player->RestoreHealth(Player->GetMaxHealth() * HealAmount);
	}
}
