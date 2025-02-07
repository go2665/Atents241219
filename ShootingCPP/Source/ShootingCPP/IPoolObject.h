// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPoolObject.generated.h"

UINTERFACE(MinimalAPI)
class UPoolObject : public UInterface
{
	GENERATED_BODY()
};

class SHOOTINGCPP_API IPoolObject
{
	GENERATED_BODY()

public:
	// 풀링 되는 오브젝트가 활성화	되었을 때 호출되는 함수
	virtual void Activate() = 0;

	// 풀링 되는 오브젝트가 비활성화 되었을 때 호출되는 함수
	virtual void Deactivate() = 0;
};
