// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EnemyHit.h"

void UAnimNotify_EnemyHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	MeshComp->SetScalarParameterValueOnMaterials(HitParamName, HitParamValue);
}
