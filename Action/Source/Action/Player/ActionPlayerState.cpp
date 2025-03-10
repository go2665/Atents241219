// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerState.h"

void AActionPlayerState::BeginPlay()
{
	Super::BeginPlay();
	OnGoldChange.AddDynamic(this, &AActionPlayerState::TestGoldChange);
}
