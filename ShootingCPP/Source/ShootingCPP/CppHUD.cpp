// Fill out your copyright notice in the Description page of Project Settings.


#include "CppHUD.h"
#include "CppGameState.h"
#include "PlayerWingUsePool.h"

void ACppHUD::BeginPlay()
{
	Super::BeginPlay();
	ShooterUserWidget = CreateWidget<UCppShooterUserWidget>(
		GetWorld(), ShooterUserWidgetClass);
	if (ShooterUserWidget)
	{
		ShooterUserWidget->AddToViewport();

		ACppGameState* GameState = GetWorld()->GetGameState<ACppGameState>();
		GameState->OnScoreChanged.AddDynamic(
			ShooterUserWidget, &UCppShooterUserWidget::RefreshScore);

		APlayerWingUsePool* Player = Cast<APlayerWingUsePool>(
			GetWorld()->GetFirstPlayerController()->GetPawn());
		Player->OnDie.AddDynamic(
			ShooterUserWidget, &UCppShooterUserWidget::ShowGameOverPanel);
	}
}
