// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor_Timeline.h"

void AFloatingActor_Timeline::BeginPlay()
{
	// 틱 함수 설정도 AActor의 BeginPlay에서 처리되므로 틱을 사용하려면 부모 클래스의 BeginPlay를 호출해야 한다.
	Super::BeginPlay();		

	FOnTimelineFloat upAndDownCallback;
	upAndDownCallback.BindUFunction(this, FName("UpdateUpAndDown"));	// 콜백 함수를 바인딩
	UpDownTimeline.AddInterpFloat(HeightCurve, upAndDownCallback);		// 타임라인에 커브와 콜백을 추가
	UpDownTimeline.SetLooping(true);	// 반복 여부
	UpDownTimeline.PlayFromStart();		// 타임라인을 처음부터 실행

	// 타임라인으로 색상 변경하기
	// 타임라인으로 회전 시키기
}

void AFloatingActor_Timeline::OnUpdateFloatMesh(float DeltaTime)
{
	UpDownTimeline.TickTimeline(DeltaTime/Duration);	// 타임라인을 업데이트
}

void AFloatingActor_Timeline::OnConstruction(const FTransform& Transform)
{
	//Super::OnConstruction(Transform);	// 부모 클래스의 함수를 호출(부모에 구현이 없음)
	
	if (MID == nullptr && StaticMesh && StaticMesh->GetMaterial(0))
	{
		MID = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), this);	// 동적 머티리얼 생성
		StaticMesh->SetMaterial(0, MID);											// 스태틱 메시에 동적 머티리얼 적용
	}
	MID->SetVectorParameterValue(FName("BaseColor"), BaseColor);	// 기본색 파라미터 설정

}

void AFloatingActor_Timeline::UpdateUpAndDown(float value)
{
	StaticMesh->SetRelativeLocation(FVector(0, 0, BaseHeight + value * MoveHeight));	// 높이 설정
}
