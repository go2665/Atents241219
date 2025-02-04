// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor_Curve.h"

void AFloatingActor_Curve::OnUpdateFloatMesh(float DeltaTime)
{
	if (HeightCurve)
	{
		// ElapsedTime이 1.0을 넘어가면 0으로 초기화(실수를 1.0으로 나눈 나머지는 소수점 이하의 값만 남는다.)
		float curveInput = FMath::Fmod(ElapsedTime, 1.0f);		
		float curveValue = HeightCurve->GetFloatValue(ElapsedTime);
		StaticMesh->SetRelativeLocation(FVector(0, 0, BaseHeight + curveValue * MoveHeight));
	}
	else
	{
		Super::OnUpdateFloatMesh(DeltaTime);
	}
}
