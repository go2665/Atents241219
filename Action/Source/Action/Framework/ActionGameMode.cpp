// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameMode.h"
#include "../Item/DropItemFactoryComponent.h"

AActionGameMode::AActionGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DropItemFactory = CreateDefaultSubobject<UDropItemFactoryComponent>(TEXT("DropItemFactoryComponent"));
}
