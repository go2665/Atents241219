// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AActionPlayerController::AActionPlayerController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();	// 로컬 플레이어 가져오기
	if (LocalPlayer)
	{
		// 로컬 플레이어가 있으면 향상된 입력 로컬 플레이어 서브시스템 가져오기
		UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSystem && DefaultContext)	// 인풋 시스템과 입력 컨택스트가 둘 다 있으면
		{			
			InputSystem->AddMappingContext(DefaultContext, 0);	// 인풋 시스템에 입력 컨택스트 추가
		}
	}
}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// InputComponent가 향상된 입력을 사용하는지 확실하지 않아 캐스팅 처리
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		if (MoveAction)	// MoveAction이 있으면
		{
			// InputMove 함수와 바인딩
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, 
				this, &AActionPlayerController::InputMove);
		}
	}
}

void AActionPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AActionPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AActionPlayerController::InputMove(const FInputActionValue& Value)
{
}
