// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SectionJump.h"
#include "../Player/ActionPlayerCharacter.h"

void UANS_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OwnerCharacter = Cast<AActionPlayerCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->OnSectionJumpReady(this);
	}
}

void UANS_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (OwnerCharacter)
	{
		OwnerCharacter->OnSectionJumpEnd();
		OwnerCharacter = nullptr;
	}
}
