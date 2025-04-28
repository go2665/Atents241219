// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferComponent.h"

UBufferComponent::UBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	ensure(BufferDataAsset != nullptr);
	
}

void UBufferComponent::AddBuffToAround()
{
}


