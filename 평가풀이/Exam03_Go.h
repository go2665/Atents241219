// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Exam03_Base.h"
#include "Exam03_Go.generated.h"


// 다이나믹 멀티캐스트 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTest1Go);

// 파라메터가 하나 있는 다이나믹 멀티캐스트 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTest2Go, float, Value);

/**
 * 
 */
UCLASS()
class ATENTS_EXAM03_API AExam03_Go : public AExam03_Base
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitEnhancedInput() override;

private:
	void OnTest1();
	void OnTest2_Press();
	void OnTest2_Release();

	FOnTest1Go OnTest1Delegate;
	FOnTest2Go OnTest2Delegate;

	float PressStartTime = 0.0f;

	UFUNCTION()
	void OnTest1Handler();
	
	UFUNCTION()
	void OnTest2Handler(float Value);
};
