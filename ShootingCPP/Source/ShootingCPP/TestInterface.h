// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)		//블루프린트에서 인식만 가능한 인터페이스
//UINTERFACE(MinimalAPI, Blueprintable)		//블루프린트에서 상속이 가능한 인터페이스
class UTestInterface : public UInterface
{
	GENERATED_BODY()
};

class SHOOTINGCPP_API ITestInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void TestNormalFunction() = 0;	// 일반 함수는 순수 가상 함수로 선언. UFUNCTION(Blueprintcallable) 매크로 사용불가.

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TestNative();
	virtual void TestNative_Implementation() = 0;	// 무조건 상속받은 클래스에서 구현해야함.
};