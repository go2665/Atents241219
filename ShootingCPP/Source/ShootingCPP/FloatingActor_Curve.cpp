// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor_Curve.h"

void AFloatingActor_Curve::OnUpdateFloatMesh(float DeltaTime)
{
	if (HeightCurve)
	{
		// ElapsedTime이 1.0을 넘어가면 0으로 초기화(실수를 1.0으로 나눈 나머지는 소수점 이하의 값만 남는다.)
		float curveInput = FMath::Fmod(ElapsedTime, 1.0f);		
		float curveValue = HeightCurve->GetFloatValue(curveInput);	// 커브의 입력이 0~1일 때의 출력값을 가져온다.
		StaticMesh->SetRelativeLocation(FVector(0, 0, BaseHeight + curveValue * MoveHeight)); // 커브의 출력값을 기준으로 높이 설정
		StaticMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));	// 회전
	}
	else
	{
		Super::OnUpdateFloatMesh(DeltaTime);
	}
}
