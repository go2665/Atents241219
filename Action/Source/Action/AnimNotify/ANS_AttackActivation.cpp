// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackActivation.h"
#include "../Player/ActionPlayerCharacter.h"

void UANS_AttackActivation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!Character)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Character is nullptr"));
		Character = Cast<AActionPlayerCharacter>(MeshComp->GetOwner());	// 처음에만 캐스팅 작업을 한다.
	}
	if (Character)
	{
		Character->SetCurrentWeaponCollisionActivate(true);
	}
}

void UANS_AttackActivation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (Character)
	{
		Character->SetCurrentWeaponCollisionActivate(false);
	}
}
