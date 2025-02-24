// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CppGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, CurrentScore);
/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API ACppGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void AddScore(int32 Point);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	FOnScoreChangedSignature OnScoreChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Score = 0;
};
