// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "NormalEnemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (EnemyCharacter == nullptr)
	{
		EnemyCharacter = Cast<ANormalEnemy>(TryGetPawnOwner());
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (EnemyCharacter)
	{
		FVector Velocity = EnemyCharacter->GetVelocity();
		Velocity.Z = 0.0f;
		Speed = Velocity.Size();
	}
	
}
