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
	// 높이 움직임을 나타내는 커브
	UPROPERTY(EditAnywhere)
	UCurveFloat* HeightCurve = nullptr;

	// 커브의 한 주기 동안의 시간
	UPROPERTY(EditAnywhere)
	float Duration = 5.0f;

	// 매 프레임마다 스테틱 메시의 움직임을 변화 시키는 함수
	virtual void OnUpdateFloatMesh(float DeltaTime) override;
};
