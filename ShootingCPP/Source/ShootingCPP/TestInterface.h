// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
//UINTERFACE(MinimalAPI, Blueprintable)
class UTestInterface : public UInterface
{
	GENERATED_BODY()
};

class SHOOTINGCPP_API ITestInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void TestNormalFunction() = 0;

	UFUNCTION(BlueprintNativeEvent)
	void TestNative();
	//virtual void TestNative_Implementation() = 0;
};