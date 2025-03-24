// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnemyHit.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UAnimNotify_EnemyHit : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
	FName HitParamName = FName("Target_Switch_On");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect", meta = (ClampMin = "0", ClampMax = "1"))
	float HitParamValue = 1.0f;
};
