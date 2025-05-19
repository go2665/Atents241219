// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ChildActorComponent.h"
#include "AreaIndicator.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Framework/TowerDefenceHUD.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Tower/TowerBuilder.h"


APlayerSpectatorPawn::APlayerSpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(Root);
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.0f));
	CameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;

}

void APlayerSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnTemporaryHeroUpdata();
}

void APlayerSpectatorPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<ATowerDefencePlayerController>(NewController);
	PlayerController->OnMouseClickInput.AddUObject(this, &APlayerSpectatorPawn::OnMouseClick); // 마우스 클릭 이벤트 바인딩
}

void APlayerSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	// AreaIndicator를 생성
	if (AreaIndicatorClass)
	{
		AreaIndicator = GetWorld()->SpawnActor<AAreaIndicator>(AreaIndicatorClass);
		if (AreaIndicator)
		{
			UE_LOG(LogTemp, Warning, TEXT("AreaIndicator spawned successfully!"));
			AreaIndicator->Deactivate();
		}
	}
}

void APlayerSpectatorPawn::OnMouseClick()
{
	//UE_LOG(LogTemp, Warning, TEXT("APlayerSpectatorPawn : Mouse Clicked!"));
	if (TemporaryHero)
	{
		ATowerBuilder* HitBuilder = nullptr;	// 클릭한 액터를 저장할 변수
		FHitResult HitResult;
		//FVector2D MousePosition;
		//FCollisionQueryParams CollisionParams;
		//CollisionParams.AddIgnoredActor(this);
		//PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
		//PlayerController->GetHitResultAtScreenPosition(
		//	MousePosition,
		//	ECC_GameTraceChannel3,
		//	CollisionParams,
		//	HitResult);	// 마우스 위치에서 히트 결과 가져오기

		if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel3, false, HitResult))	// ECC_GameTraceChannel3(TowerBuilder)로 트레이스
		{
			AActor* HitActor = HitResult.GetActor();	// 클릭한 액터 저장
			//UE_LOG(LogTemp, Warning, TEXT("Clicked on: %s"), *HitActor->GetActorNameOrLabel());
			HitBuilder = Cast<ATowerBuilder>(HitResult.GetActor());
			if (HitBuilder)
			{
				//UE_LOG(LogTemp, Warning, TEXT("FindBuilder!"));
				bool bResult = HitBuilder->SetTowerOnce(TemporaryHero);	// 타워 빌더에 타워 설정
				if(bResult)
				{
					OnHeroTowerBuildComplete.ExecuteIfBound(TemporaryHero);	// 타워 건축 완료 델리게이트 호출
					TemporaryHero = nullptr;
				}
			}
		}
	}
}

void APlayerSpectatorPawn::OnTemporaryHeroUpdata()
{
	if (TemporaryHero && PlayerController)
	{
		FHitResult HitResult;

		FVector2D MousePosition;
		FCollisionQueryParams CollisionParams;		
		CollisionParams.AddIgnoredActor(TemporaryHero); // 타워를 무시하도록 설정		
		CollisionParams.bTraceComplex = false;

		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

		if (PlayerController->GetHitResultAtScreenPosition(
			MousePosition,
			ECollisionChannel::ECC_Visibility,
			CollisionParams,
			HitResult))
		{
			TemporaryHero->SetActorLocation(HitResult.ImpactPoint); // AreaIndicator 위치로 타워 이동
		}
	}
}
