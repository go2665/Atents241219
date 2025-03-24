// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_EnemyAttackActivation.h"
#include "Action/Enemy/NormalEnemy.h"

void UANS_EnemyAttackActivation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!Enemy)
	{
		Enemy = Cast<ANormalEnemy>(MeshComp->GetOwner());
	}

	if (Enemy)
	{
		Enemy->SetWeaponCollisionActivate(true);
	}
}

void UANS_EnemyAttackActivation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Enemy)
	{
		Enemy->SetWeaponCollisionActivate(false);
	}
}
