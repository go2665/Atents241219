// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UBTDecorator_DistanceCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_DistanceCheck();

protected:
	// 비헤이비어 트리 에셋이 초기화될 때 한번 호출
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	// 블랙보드의 값을 판별할 때 호출
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float DistanceThreshold = 150.0f;

private:
	float SquareDistanceThreshold = 0.0f;
};
