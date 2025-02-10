// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IPoolObject.h"
#include "PoolingObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeactivateDelegate, APoolingObjectBase*, DeactivatedObject);

UCLASS(Blueprintable)
class SHOOTINGCPP_API APoolingObjectBase : public AActor, public IPoolObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolingObjectBase();

public:	
	// 활성화 되었을 때 호출되는 함수
	virtual void Activate() override;
	// 비활성화 되었을 때 호출되는 함수
	virtual void Deactivate() override;

	// 비활성화를 알리는 델리게이트
	FOnDeactivateDelegate OnObjectDeactivate;

protected:
	// 파생 클래스용 활성화 함수
	virtual void OnActivate();
	// 파생 클래스용 비활성화 함수
	virtual void OnDeactivate();


};
