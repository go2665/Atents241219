// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWing.h"
#include "Components/ArrowComponent.h"

// Sets default values
APlayerWing::APlayerWing()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	arrow->SetupAttachment(arrow);

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));	
	FloatingMovement->MaxSpeed = 500;
	FloatingMovement->Acceleration = 300;
	FloatingMovement->Deceleration = 100;
	FloatingMovement->TurningBoost = 0;
	FloatingMovement->bConstrainToPlane = true;
	FloatingMovement->ConstrainNormalToPlane(FVector::UpVector);

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

		if (GeoFireNormal)
		{
			Input->BindAction(GeoFireNormal, ETriggerEvent::Triggered, this, &APlayerWing::GeoInputFireNormal);
		}
		if (GeoFireHomming)
		{
			Input->BindAction(GeoFireHomming, ETriggerEvent::Started, this, &APlayerWing::GeoInputFireHomming);
			Input->BindAction(GeoFireHomming, ETriggerEvent::Completed, this, &APlayerWing::GeoInputFireHomming);
		}
		if (GeoFireArea)
		{
			Input->BindAction(GeoFireArea, ETriggerEvent::Started, this, &APlayerWing::GeoInputFireArea);
			Input->BindAction(GeoFireArea, ETriggerEvent::Completed, this, &APlayerWing::GeoInputFireArea);
		}
	}

}

void APlayerWing::GeoInputMove(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
		FString::Printf(TEXT("GeoInputMove : %.1f, %.1f"), InputValue.X, InputValue.Y));*/

	InputValue.Normalize();			// 노멀벡터를 계산한 후 자신에게 적용
	//InputValue.GetSafeNormal();	// 노멀벡터를 계산한 후 새 벡터를 만들어서 리턴

	FVector inputDirection(InputValue.Y, InputValue.X, 0);	// 월드 방향에 맞게 FVector로 이동 방향 생성
	AddMovementInput(inputDirection);		// 이동 방향 적용
}

void APlayerWing::GeoInputFireNormal(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireNormal"));
}

void APlayerWing::GeoInputFireHomming(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireHomming"));
}

void APlayerWing::GeoInputFireArea(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireArea"));
}

