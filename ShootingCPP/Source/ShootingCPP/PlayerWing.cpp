// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWing.h"

// Sets default values
APlayerWing::APlayerWing()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerWing::BeginPlay()
{
	Super::BeginPlay();

	AController* WingController = GetController();
	APlayerController* PlayerWingController = Cast<APlayerController>(WingController);
	ULocalPlayer* LocalPlayer = PlayerWingController->GetLocalPlayer();	

	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem != nullptr && DefaultMappingContext != nullptr)
	{
		InputSystem->AddMappingContext(DefaultMappingContext, 0);	// 향상된 입력과 Context 연결
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input System is null"));
	}
}

// Called every frame
void APlayerWing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerWing::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (Input)	// Input != nullptr
	{
		if (GeoMoveAction)
		{
			Input->BindAction(GeoMoveAction, ETriggerEvent::Triggered, this, &APlayerWing::GeoInputMove);
		}
	}

}

void APlayerWing::GeoInputMove(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
		FString::Printf(TEXT("GeoInputMove : %.1f, %.1f"), InputValue.X, InputValue.Y));
}

