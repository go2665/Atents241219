// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UActionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 애니메이션이 초기화 될 때 실행
	virtual void NativeInitializeAnimation() override;

	// 매 틱마다 실행(데이터 가져오는 용도)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float Speed = 0.0f;

	UPROPERTY()
	class AActionPlayerCharacter* PlayerCharacter = nullptr;
};
