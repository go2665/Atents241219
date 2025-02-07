// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "TestInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTestInterface : public UInterface
{
	GENERATED_BODY()
};

class ITestInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void TestNormalFunction();

	UFUNCTION(BlueprintNativeEvent)
	void TestNative();
};