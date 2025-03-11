// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameMode.h"

AActionGameMode::AActionGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	DropItemFactory = CreateDefaultSubobject<UDropItemFactoryComponent>(TEXT("DropItemFactoryComponent"));
}
