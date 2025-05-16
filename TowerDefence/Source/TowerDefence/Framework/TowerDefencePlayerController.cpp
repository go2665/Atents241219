// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefencePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


ATowerDefencePlayerController::ATowerDefencePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ATowerDefencePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 향상된 입력 사용	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// 마우스 클릭 이벤트 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(
			ClickAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::OnMouseClick);
	}
}

void ATowerDefencePlayerController::OnMouseClick(const FInputActionValue& Value)
{	
	UE_LOG(LogTemp, Warning, TEXT("ATowerDefencePlayerController : Mouse Clicked!"));	// 마우스 클릭 로그 출력

	// 커서 위치에서 (0,0,1)방향으로 라인트레이스
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked on: %s"), *HitActor->GetActorNameOrLabel());
		}
	}

	OnMouseClickInput.Broadcast();	// 마우스 클릭 델리게이트 호출
}