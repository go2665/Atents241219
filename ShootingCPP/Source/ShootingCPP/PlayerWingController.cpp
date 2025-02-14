// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWingController.h"
#include "PlayerWingUsePool.h"

APlayerWingController::APlayerWingController()
{
	//bShowMouseCursor = true;
	SetShowMouseCursor(true);	// 변수로 세팅하는 것에 비해 로그만 추가로 찍어준다.
	DefaultMouseCursor = EMouseCursor::Crosshairs;	// 크로스헤어로 마우스 커서 바꾸기
													// (입력 초기화하면서 CurrentMouseCusor가 DefaultMouseCusor값으로 수정된다.)
	
}

void APlayerWingController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APlayerWingUsePool* PlayerWing = Cast<APlayerWingUsePool>(InPawn);
	if (PlayerWing)
	{
		PlayerWing->OnDie.AddDynamic(this, &APlayerWingController::OnPlayerDie);
	}
}

void APlayerWingController::OnPlayerDie()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	APlayerWingUsePool* PlayerWing = Cast<APlayerWingUsePool>(GetPawn());
	InputSubSystem->RemoveMappingContext(PlayerWing->GetMappingContext());
}
