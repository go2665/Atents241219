// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWing.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"

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
	arrow->SetupAttachment(RootComponent);

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

	AController* wingController = GetController();
	APlayerController* playerWingController = Cast<APlayerController>(wingController);
	ULocalPlayer* localPlayer = playerWingController->GetLocalPlayer();

	if (localPlayer)
	{
		// 입력 컨텍스트 연결하기
		UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (inputSystem != nullptr && DefaultMappingContext != nullptr)
		{
			inputSystem->AddMappingContext(DefaultMappingContext, 0);	// 향상된 입력과 Context 연결
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Input System is null"));
		}
	}

	// 보더 위치 결정
	UpdateOrthoSize();
}

// Called every frame
void APlayerWing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookMouseLocation(DeltaTime);
	WarpToOtherSide();
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
			Input->BindAction(GeoFireNormal, ETriggerEvent::Started, this, &APlayerWing::GeoInputFireNormal_Press);
			Input->BindAction(GeoFireNormal, ETriggerEvent::Completed, this, &APlayerWing::GeoInputFireNormal_Release);
		}
		if (GeoFireHomming)
		{
			Input->BindAction(GeoFireHomming, ETriggerEvent::Triggered, this, &APlayerWing::GeoInputFireHomming);
		}
		if (GeoFireArea)
		{
			Input->BindAction(GeoFireArea, ETriggerEvent::Started, this, &APlayerWing::GeoInputFireArea, true);
			Input->BindAction(GeoFireArea, ETriggerEvent::Completed, this, &APlayerWing::GeoInputFireArea, false);
		}
	}
}

void APlayerWing::OnFireStart(EProjectileType Type)
{
	switch (Type)
	{
	case APlayerWing::EProjectileType::Normal:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireNormal - Press"));
		break;
	case APlayerWing::EProjectileType::Homing:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireHoming - Press"));
		break;
	case APlayerWing::EProjectileType::Area:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireArea - Press"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ERROR - OnFireStart"));
		break;
	}
}

void APlayerWing::OnFireEnd(EProjectileType Type)
{
	switch (Type)
	{
	case APlayerWing::EProjectileType::Normal:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireNormal - Release"));
		break;
	case APlayerWing::EProjectileType::Homing:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireHoming - Release"));
		break;
	case APlayerWing::EProjectileType::Area:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GeoInputFireArea - Release"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ERROR - OnFireEnd"));
		break;
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

void APlayerWing::GeoInputFireNormal_Press(const FInputActionValue& Value)
{
	OnFireStart(EProjectileType::Normal);
}

void APlayerWing::GeoInputFireNormal_Release(const FInputActionValue& Value)
{
	OnFireEnd(EProjectileType::Normal);
}

void APlayerWing::GeoInputFireHomming(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
		OnFireStart(EProjectileType::Homing);
	else
		OnFireEnd(EProjectileType::Homing);
}

void APlayerWing::GeoInputFireArea(const FInputActionValue& Value, bool bPressed)
{
	if (bPressed)
		OnFireStart(EProjectileType::Area);
	else
		OnFireEnd(EProjectileType::Area);
}

void APlayerWing::UpdateOrthoSize()
{
	AController* wingController = GetController();
	APlayerController* playerWingController = Cast<APlayerController>(wingController);

	AActor* viewTarget = playerWingController->GetViewTarget();
	UCameraComponent* cameraComponent = viewTarget->FindComponentByClass<UCameraComponent>();
	float orthoSize = cameraComponent->OrthoWidth;

	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green,
	//	FString::Printf(TEXT("Playerwing orthoSize : %.1f"), orthoSize));

	BorderX = orthoSize * 0.5f + PlayerMargin;
	BorderY = orthoSize * 0.5f * 1.77777f + PlayerMargin;

	/*GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green,
		FString::Printf(TEXT("Playerwing Border : %.1f, %.1f"), BorderX, BorderY));*/
}

void APlayerWing::LookMouseLocation(float InDeltaTime)
{
	// 기본 데이터 가져오기
	FVector playerLocation = GetActorLocation();		// 플레이어 위치 가져오기	
	UWorld* world = GetWorld();
	APlayerController* controller = world->GetFirstPlayerController();	// 마우스 hit 위치를 계산하기 위해 컨트롤러 가져오기
	FHitResult hitResult;
	controller->GetHitResultUnderCursorByChannel(
		ETraceTypeQuery::TraceTypeQuery1, true, hitResult);				// 마우스 커서가 있는 위치 구하기
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
		FString::Printf(TEXT("Hit : %.1f, %.1f, %.1f"), hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z));*/
		
	// 커서 위치 확정(XY는 hit된 위치, 높이는 플레이어와 동일한 높이로 설정)
	FVector cursorLocation(hitResult.Location.X, hitResult.Location.Y, playerLocation.Z);	

	// 플레이어의 위치와 커서의 위치를 바탕으로 방향 계산
	FVector direction = (cursorLocation - playerLocation).GetSafeNormal();	// 플레이어 위치에서 마우스 커서 위치로 가는 방향 벡터 구하기		

	// 방향을 기반으로 최종 목표회전 결정
	FRotator toRotator = direction.Rotation();		// direction방향으로 바라보는 회전 만들기
	FRotator playerRotator = GetActorRotation();	// 플레이어의 회전

	// 플레이어의 회전에서 최종 목표회전으로 보간
	FRotator interpToRotator = FMath::RInterpTo(playerRotator, toRotator, InDeltaTime, TurnSpeed);	// 보간된 회전

	// 보간된 결과를 플레이어에게 적용
	SetActorRotation(interpToRotator);
}

void APlayerWing::WarpToOtherSide()
{
	FVector playerLocation = GetActorLocation();
	if (BorderX < playerLocation.X || -BorderX > playerLocation.X)
	{
		GetOtherSideLocation(playerLocation);	// 현재 위치의 반대 위치 구하기

		// playerLocation를 중앙쪽으로 유닛 벡터만큼 이동
		FVector BasePosition = FVector(0, 0, playerLocation.Z);
		FVector Direction = (BasePosition - playerLocation).GetSafeNormal();
		playerLocation += Direction * 10.0f;

		SetActorLocation(playerLocation, false, nullptr, ETeleportType::TeleportPhysics);	// 최종적으로 반대 위치로 이동
	}

	if (BorderY < playerLocation.Y || -BorderY > playerLocation.Y)
	{
		GetOtherSideLocation(playerLocation);
		SetActorLocation(playerLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

