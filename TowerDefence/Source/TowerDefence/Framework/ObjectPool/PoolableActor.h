// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "PoolableActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 오브젝트 풀에서 사용할 오브젝트가 상속받을 인터페이스(선택)
 */
class TOWERDEFENCE_API IPoolableActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 오브젝트가 풀에서 생성될 때 호출(초기화)
	virtual void OnInitialize() = 0;

	// 오브젝트가 풀에서 꺼내질 때 호출(활성화 때)
	virtual void OnActivate() = 0;

	// 오브젝트가 풀로 돌아갈 때 호출(비활성화 때)
	virtual void OnDeactivate() = 0;

	// 풀 타입을 가져온다.
	virtual EPooledActorType GetPoolType() const = 0;

	// 풀 타입을 설정한다.
	virtual void SetPoolType(EPooledActorType InType) = 0;
};
