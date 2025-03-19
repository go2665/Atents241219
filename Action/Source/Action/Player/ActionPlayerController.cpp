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
		if (InputSystem && MovementContext)	// 인풋 시스템과 입력 컨택스트가 둘 다 있으면
		{			
			InputSystem->AddMappingContext(MovementContext, 0);	// 인풋 시스템에 입력 컨택스트 추가
		}
	}

	PlayerCameraManager->ViewPitchMin = CameraPitchMin;
	PlayerCameraManager->ViewPitchMax = CameraPitchMax;

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

		if (LookAction)	// LookAction이 있으면
		{
			// InputLook 함수와 바인딩
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,
				this, &AActionPlayerController::InputLook);
		}

		if (SprintAction)	// LookAction이 있으면
		{
			// InputLook 함수와 바인딩
			EnhancedInputComponent->BindActionValueLambda(SprintAction, ETriggerEvent::Started,
				[this](const FInputActionValue& Value) {
					InputSprint(true);
				});

			EnhancedInputComponent->BindActionValueLambda(SprintAction, ETriggerEvent::Completed,
				[this](const FInputActionValue& Value) {
					InputSprint(false);
				});
		}

		if (RollAction)	// RollAction이 있으면
		{
			// InputRoll 함수와 바인딩
			EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started,
				this, &AActionPlayerController::InputRoll);
		}

		if (AttackAction)	// AttackAction이 있으면
		{
			// InputAttack 함수와 바인딩
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started,
				this, &AActionPlayerController::InputAttack);
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

		FVector Direction(InputValue.Y, InputValue.X, 0.0f);	// 입력 받은 대로 방향 설정

		// 카메라 방향을 기준으로 이동시키기		
		FQuat ControlYawRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f).Quaternion(); // 컨트롤러의 회전값(카메라의 회전)을 가져와서 Yaw값만 사용하여 쿼터니언 만들기
		Direction = ControlYawRotation.RotateVector(Direction);	// 입력방향을 카메라 Yaw 회전값에 맞게 회전		

		// 캐릭터에게 방향 전달
		PlayerCharacter->Movement(Direction);

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan,
		//	FString::Printf(TEXT("InputMove: %s"), *Direction.ToString()));
	}
}

void AActionPlayerController::InputLook(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddYawInput(InputValue.X);
	AddPitchInput(InputValue.Y);
}

void AActionPlayerController::InputSprint(bool IsPress)
{
	if (PlayerCharacter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::Printf(TEXT("InputSprint: %s"), IsPress ? TEXT("True") : TEXT("False")));
		if (IsPress)
		{
			PlayerCharacter->SetSprintMode();	// 누르고 있는 상태면 달리기 모드로 설정
		}
		else
		{
			PlayerCharacter->SetWalkMode();		// 뗀 상태면 걷기 모드로 되돌리기
		}
	}
}

void AActionPlayerController::InputRoll(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("InputRoll"));
	if (PlayerCharacter)
	{
		PlayerCharacter->DoRoll();
	}
}

void AActionPlayerController::InputAttack(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("InputAttack"));
	if (PlayerCharacter)
	{
		PlayerCharacter->DoAttack();
	}
}

void AActionPlayerController::InputTest(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("InputTest"));
}
