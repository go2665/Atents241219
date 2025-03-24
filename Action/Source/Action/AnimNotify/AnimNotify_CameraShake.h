// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	float ShakeScale = 1.0f;

private:
	UPROPERTY()
	class APlayerController* PlayerController = nullptr;
};
