// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CameraShake.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!PlayerController)
	{
		APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
		if (Pawn)
		{
			PlayerController = Cast<APlayerController>(Pawn->GetController());
		}
	}

	if (CameraShakeClass && PlayerController)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, ShakeScale);
	}
}
