// Fill out your copyright notice in the Description page of Project Settings.


#include "CppShooterUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "PlayerWingUsePool.h"

void UCppShooterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(0));
	}

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	Player = Cast<APlayerWingUsePool>(PlayerController->GetPawn());
}

void UCppShooterUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TargetScore > CurrentScore)
	{
		CurrentScore += IncreaseSpeedModifier * InDeltaTime;

		//CurrentScore = FMath::Min(CurrentScore, TargetScore); // 아래와 같은 코드
		if (CurrentScore > TargetScore)
		{
			CurrentScore = TargetScore;
		}
		
		ScoreText->SetText(FText::AsNumber(FMath::FloorToInt(CurrentScore)));	// 소수점 아래 날리기
	}

	if (Player)
	{
		HealthPercent = Player->GetHealth() / Player->GetMaxHealth();
	}
}

void UCppShooterUserWidget::RefreshScore(int32 Point)
{
	TargetScore = Point;
	IncreaseSpeedModifier = FMath::Max(MinimumIncreaseSpeed, TargetScore - CurrentScore);
}

void UCppShooterUserWidget::ShowGameOverPanel()
{
	if (GameOverPanel)
	{
		GameOverPanel->SetVisibility(ESlateVisibility::Visible);
	}
}
