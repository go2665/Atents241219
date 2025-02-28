// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AreaDamage.generated.h"

/**
 * 범위 데미지를 주는 노티파이
 */
UCLASS()
class ACTION_API UAnimNotify_AreaDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaDamage")
	float CenterLength = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaDamage")
	float InnerRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaDamage")
	float OuterRadius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaDamage")
	float FallOff = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaDamage")
	float DebugDuration = 1.0f;

private:
	UPROPERTY()
	class AActionPlayerCharacter* PlayerCharacter = nullptr;
};
