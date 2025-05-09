// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpectatorPawn.h"
#include "Components/ChildActorComponent.h"
#include "AreaIndicator.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Tower/Tower.h"


APlayerSpectatorPawn::APlayerSpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = true;
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
	UE_LOG(LogTemp, Warning, TEXT("Clicked!"));
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
