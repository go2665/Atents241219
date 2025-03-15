// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_Pickup.h"
#include "../ItemData/ItemDataAsset.h"
#include "../../Player/ActionPlayerState.h"

void ADropItem_Pickup::OnGetItem(AActor* InTarget)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,
	//	FString::Printf(TEXT("ADropItem_Pickup::OnGetItem - %s"), *ItemDataAsset->ItemName.ToString()));

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			AActionPlayerState* PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();
			if (PlayerState)
			{
				if (PlayerState->AddItemToInventory(ItemDataAsset))	// 인벤토리에 추가 성공하면
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,
						FString::Printf(TEXT("Add Item to Inventory : %s, Success "), *ItemDataAsset->ItemName.ToString()));
					Destroy();	// 아이템 제거
				}
			}
		}
	}
}
