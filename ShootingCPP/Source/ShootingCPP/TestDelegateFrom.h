// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestDelegateFrom.generated.h"

// 델리게이트나 이벤트의 선언을 먼저 해야 한다.
DECLARE_DELEGATE(FTestDelegate1);			// 기본 델리게이트 void 반환형, 파라메터 없음
DECLARE_DYNAMIC_DELEGATE(FTestDelegate2);	// 델리게이트 동적 바인딩. 블루프린트에서 사용 가능. UFUNCTION이 붙은 함수만 바인딩 가능
DECLARE_MULTICAST_DELEGATE(FTestDelegate3);	// 멀티캐스트 델리게이트. 여러 개의 리스너에게 이벤트를 전달할 수 있음
DECLARE_EVENT(ATestDelegateFrom, FTestDelegate4);	// 이벤트 델리게이트. 클래스 밖으로 신호를 보낼 수 없다. 
													// 리스터가 여럿 있을 수 있다. 블루프린트에서 사용 불가능. 리턴값이 없다.
DECLARE_DELEGATE_OneParam(FTestDelegate5, int32);	// 파라메터가 하나인 델리게이트(파라메터는 int32)
DECLARE_DELEGATE_RetVal_OneParam(float, FTestDelegate6, int32);	// 리턴값이 있는 델리게이트(리턴값은 float, 파라메터는 int32)

UCLASS()
class SHOOTINGCPP_API ATestDelegateFrom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestDelegateFrom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FTestDelegate3 OnFinish;	// 멀티캐스트 델리게이트 선언. (디스패치 만든 것과 같음)

	void TestRunDelegate();	// 델리게이트 실행 함수

};
