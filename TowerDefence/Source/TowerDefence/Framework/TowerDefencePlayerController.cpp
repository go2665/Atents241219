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
	AActor* HitActor = nullptr;	// 클릭한 액터를 저장할 변수
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult))	// ECC_GameTraceChannel2로 트레이스
	{
		HitActor = HitResult.GetActor();
	}
	//if (HitActor)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Clicked on: %s"), *HitActor->GetActorNameOrLabel());
	//}
	OnMouseClickInput.Broadcast(HitActor);	// 마우스 클릭 델리게이트 호출

}
