// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_EnemyAttackActivation.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UANS_EnemyAttackActivation : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// 노티파이 시작시 호출
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// 노티파이가 끝날 때 호출
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	class ANormalEnemy* Enemy = nullptr;
};
