// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionPlayerCharacter.h"


AActionPlayerController::AActionPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;			// 틱을 사용하지 않으면 입력처리가 안된다.
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

		if (TestAction)
		{
			EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Started,
				this, &AActionPlayerController::InputTest);
		}
	}
}

void AActionPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("OnPossess"));
	PlayerCharacter = Cast<AActionPlayerCharacter>(InPawn);
}

void AActionPlayerController::OnUnPossess()
{
	PlayerCharacter = nullptr;
	Super::OnUnPossess();
}

void AActionPlayerController::InputMove(const FInputActionValue& Value)
{
	if (PlayerCharacter)	// 조종하는 캐릭터가 있으면 처리
	{
		FVector2D InputValue = Value.Get<FVector2D>();
		
		float Size = InputValue.Size();
		if (Size > 1.0f)		// 입력값이 1보다 크면(= 일정 속도 이상의 가속도는 금지)
		{
			InputValue /= Size;	// 정규화 == 유닛 벡터화
		}

		FVector Direction(InputValue.Y, InputValue.X, 0.0f);

		PlayerCharacter->Movement(Direction);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan,
			FString::Printf(TEXT("InputMove: %s"), *Direction.ToString()));
	}
}

void AActionPlayerController::InputTest(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("InputTest"));
}
