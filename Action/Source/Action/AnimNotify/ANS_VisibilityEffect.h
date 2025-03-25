// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_VisibilityEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UANS_VisibilityEffect : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisibilityEffect")
	FName CurveName = FName(TEXT("Dissolve"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisibilityEffect")
	FName MaterialParamName = FName(TEXT("Visibility"));
};
