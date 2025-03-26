// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Normal.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AAIController_Normal : public AAIController
{
	GENERATED_BODY()
public:
	void StopBehaviorTree();

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;
};
