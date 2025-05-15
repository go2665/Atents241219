// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEnd.h"

void UGameEnd::Open()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimation(StartAnimation);
}

void UGameEnd::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}
