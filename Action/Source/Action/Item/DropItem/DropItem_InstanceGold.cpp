// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_InstanceGold.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Action/Player/ActionPlayerState.h"
#include "Action/Player/ActionPlayerCharacter.h"

void ADropItem_InstanceGold::OnGetItem(AActor* InTarget)
{
	// 먹을 때 태그로 확인하고 있어서 꼭 필요는 없음
	//AActionPlayerCharacter* Player = Cast<AActionPlayerCharacter>(InTarget);	
	//if (Player)
	{
		// 플레이어 스테이트 가져오기
		UWorld* World = GetWorld();
		APlayerController* PlayerController = World->GetFirstPlayerController();
		AActionPlayerState* PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();

		PlayerState->AddGold(ItemDataAsset->ItemPrice);

		Destroy();
	}
}
