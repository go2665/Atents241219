// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAnimInstance.h"
#include "ActionPlayerCharacter.h"

void UActionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerCharacter = Cast<AActionPlayerCharacter>(TryGetPawnOwner());
}

void UActionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter)
	{
		Speed = PlayerCharacter->GetVelocity().Size();
	}
}
