// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingActor_Curve.h"
#include "Components/TimelineComponent.h"

#include "FloatingActor_Timeline.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API AFloatingActor_Timeline : public AFloatingActor_Curve
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;	
	virtual void OnUpdateFloatMesh(float DeltaTime) override;

public:
	// 생성될 때나 에디터에서 변경할 때 호출됨
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	/// <summary>
	/// 타임라인이 실행될 때마다 호출되는 함수.
	/// </summary>
	/// <param name="value">타임라인의 커브에서 사용할 입력값</param>
	UFUNCTION()
	void UpdateUpAndDown(float value);	// 타임라인에서 호출하기 위해서는 UFUNCTION() 매크로를 사용해야 한다.

	UFUNCTION()
	void UpdateColor(float value);

	UFUNCTION()
	void UpdateSpin(float value);

protected:
	UPROPERTY(EditAnywhere)
	FLinearColor BaseColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	FLinearColor SecondColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ColorCurve = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveFloat* SpinCurve = nullptr;

private:
	// 사용할 타임라인 
	UPROPERTY()
	FTimeline UpDownTimeline;

	UPROPERTY()
	FTimeline ColorTimeline;

	UPROPERTY()
	FTimeline SpinTimeline;

	// 이 액터의 머티리얼 인스턴스
	UPROPERTY()
	UMaterialInstanceDynamic* MID = nullptr;
};
