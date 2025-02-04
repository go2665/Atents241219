// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingActorBase.h"
#include "FloatingActor_Curve.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API AFloatingActor_Curve : public AFloatingActorBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	UCurveFloat* HeightCurve = nullptr;

	virtual void OnUpdateFloatMesh(float DeltaTime) override;
};
