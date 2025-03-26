// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameMode.h"
#include "MainHUD.h"

AActionGameMode::AActionGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	DropItemFactory = CreateDefaultSubobject<UDropItemFactoryComponent>(TEXT("DropItemFactoryComponent"));
}

void AActionGameMode::OnGameClear()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->DisableInput(PlayerController);	// 입력 비활성화

	AMainHUD* MainHUD = PlayerController->GetHUD<AMainHUD>();
	MainHUD->GameClear();	// 게임 클리어 UI 표시
}
