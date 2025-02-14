// Fill out your copyright notice in the Description page of Project Settings.
// 이름
#pragma once

#include "CoreMinimal.h"
#include "Exam02_Base.h"
#include "Exam02_Go.generated.h"

/**
 * 
 */
UCLASS()
class ATENTS_EXAM02_API AExam02_Go : public AExam02_Base
{
	GENERATED_BODY()
	
public:
	AExam02_Go();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 입력 액션 바인딩용 함수
	virtual void OnPlayPauseInputAction(const FInputActionValue& Input) override;

	// 토글	재생/일시정지
	virtual void TogglePlayPause() override;

	// 향상된 입력 초기화
	virtual void InitEnhancedInput() override;

	// 이동 처리
	virtual void Tick_Movement(float DeltaTime) override;
};
