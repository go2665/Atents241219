// Fill out your copyright notice in the Description page of Project Settings.
// 이름

#include "Exam02_Go.h"

/*
[요구조건]
1. h파일과 cpp파일의 첫줄에 본인 이름을 주석으로 작성
2. 컴포넌트의 계층구조 설정
	2-1. StaticMeshComponent3은 StaticMeshComponent2의 자식이어야 한다.
	2-2. StaticMeshComponent2는 StaticMeshComponent1의 자식이어야 한다.
3. 향상된 입력 처리
	3-1. 매핑 컨텍스트는 InputMappingContext를 사용한다.
	3-2. 인풋 액션은 PlayPauseInputAction를 사용한다.
	3-3. PlayPauseInputAction액션이 Started될 때 OnPlayPauseInputAction 함수가 실행된다.
	3-4. 매핑 컨텍스트에 설정된 키가 눌려질 때 마다 움직임이 정지하거나 다시 진행된다.
4. 컴포넌트의 이동 및 회전
	4-1. StaticMeshComponent2는 StaticMeshComponent1를 중심으로 500만큼 떨어진 위치에서 초당 50도의 속도로 한 바퀴를 돈다.
	4-2. StaticMeshComponent3는 StaticMeshComponent2를 중심으로 200만큼 떨어진 위치에서 초당 100도의 속도로 한 바퀴를 돈다.
	4-3. StaticMeshComponent2와 StaticMeshComponent3은 Yaw회전만 해야 한다.
*/

AExam02_Go::AExam02_Go()
{
	PrimaryActorTick.bCanEverTick = true;

	// 2. 컴포넌트의 계층구조 설정
	StaticMeshComponent2->SetupAttachment(StaticMeshComponent1);
	StaticMeshComponent3->SetupAttachment(StaticMeshComponent2);
}

void AExam02_Go::BeginPlay()
{
	Super::BeginPlay();

	InitEnhancedInput();
}

void AExam02_Go::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlaying)
	{
		Tick_Movement(DeltaTime);
	}
}

void AExam02_Go::OnPlayPauseInputAction(const FInputActionValue& Input)
{
	TogglePlayPause();
}

void AExam02_Go::TogglePlayPause()
{
	bIsPlaying = !bIsPlaying;
}

void AExam02_Go::InitEnhancedInput()
{
	// 3. 향상된 입력 처리
	// 매핑 컨텍스트 = 키와 인풋액션(InputAction)을 연결
	// 인풋액션과 함수 바인딩 = 인풋액션(InputAction)이 트리거 되었을 때 실행될 함수 지정

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = 
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (EnhancedInputSubsystem && InputMappingContext)
			{
				EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);	// 매핑 컨텍스트 서브시스템에 연결

				//액터의 InputComponent를 사용하는 경우 어떤 컨트롤러로 활성화 할지 설정해야 한다.
				//EnableInput(PlayerController);	// 인풋 활성화

				UEnhancedInputComponent* EnhancedInputComponent = 
					Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
				
				if (EnhancedInputComponent)
				{
					// 인풋액션과 함수 바인딩
					EnhancedInputComponent->BindAction(PlayPauseInputAction, ETriggerEvent::Started, 
						this, &AExam02_Go::OnPlayPauseInputAction);
				}
			}

		}
	}
}

void AExam02_Go::Tick_Movement(float DeltaTime)
{
	// Yaw회전 반복 수행(초당 50도)
	StaticMeshComponent2->AddRelativeRotation(FRotator(0.0f, 50.0f * DeltaTime, 0.0f));				
	// 앞쪽 방향으로 500만큼 떨어진 위치로 이동
	StaticMeshComponent2->SetRelativeLocation(StaticMeshComponent2->GetForwardVector() * 500.0f);	

	// Yaw회전 반복 수행(초당 100도)
	StaticMeshComponent3->AddRelativeRotation(FRotator(0.0f, 100.0f * DeltaTime, 0.0f));
	// 앞쪽 방향으로 200만큼 떨어진 위치로 이동
	StaticMeshComponent3->SetRelativeLocation(StaticMeshComponent3->GetForwardVector() * 200.0f);
}
