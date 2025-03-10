// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_InstanceGold.h"
#include "Action/Item/ItemDataAsset.h"
#include "Action/Player/ActionPlayerState.h"

void ADropItem_InstanceGold::OnGetItem(AActor* Target)
{
	// 플레이어 스테이트 가져오기
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	AActionPlayerState* PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();

	PlayerState->AddGold(ItemDataAsset->ItemPrice);
}
