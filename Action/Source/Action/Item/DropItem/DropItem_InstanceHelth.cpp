// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_InstanceHelth.h"
#include "../../Player/ActionPlayerCharacter.h"
#include "../ItemData/ItemDataAsset_Heal.h"

void ADropItem_InstanceHelth::OnGetItem(AActor* InTarget)
{
	AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(InTarget);
	UItemDataAsset_Heal* HealData = Cast<UItemDataAsset_Heal>(ItemDataAsset);
	if (Player && HealData)	// 아이템 사용 대상이 플레이어고 이 아이템의 데이터가 회복 아이템이라면
	{
		Player->RestoreHealth(HealData->HealAmount, HealData->HealDuration);	// 플레이어의 체력 회복 처리

		Destroy();
	}
}
