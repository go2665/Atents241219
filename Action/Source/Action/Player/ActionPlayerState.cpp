// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerState.h"
#include "ActionPlayerCharacter.h"

AActionPlayerState::AActionPlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AActionPlayerState::BeginPlay()
{
	Super::BeginPlay();
	InventoryComponent->InitInventory(Cast<AActionPlayerCharacter>(this->GetPawn()));
}
