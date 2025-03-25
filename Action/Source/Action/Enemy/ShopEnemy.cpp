// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopEnemy.h"
#include "GameFramework/GameModeBase.h"
#include "Action/Framework/MainHUD.h"

void AShopEnemy::Die()
{
	// 상점 열기
	UWorld* World = GetWorld();
	AGameModeBase* GameMode = World->GetAuthGameMode();
	
	APlayerController* PlayerController =  World->GetFirstPlayerController();
	AMainHUD* Hud = PlayerController->GetHUD<AMainHUD>();
	Hud->OpenShop();


	// 자신을 파괴
	Destroy();
}
