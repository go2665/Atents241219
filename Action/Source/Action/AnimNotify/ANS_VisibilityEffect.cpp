// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_VisibilityEffect.h"

void UANS_VisibilityEffect::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		float CurveValue = MeshComp->GetAnimInstance()->GetCurveValue(CurveName);
		MeshComp->SetScalarParameterValueOnMaterials(MaterialParamName, CurveValue);
	}
}

