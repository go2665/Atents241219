// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingObjectBase.h"
#include "TestPoolingObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTINGCPP_API ATestPoolingObject : public APoolingObjectBase
{
	GENERATED_BODY()
	
public:
	ATestPoolingObject();

	UFUNCTION(BlueprintCallable)
	void DeactivateTest();

protected:
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	UStaticMeshComponent* Mesh = nullptr;
};
