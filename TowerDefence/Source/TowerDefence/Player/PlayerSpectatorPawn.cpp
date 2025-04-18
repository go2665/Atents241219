// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpectatorPawn.h"
#include "Components/ChildActorComponent.h"
#include "AreaIndicator.h"


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
