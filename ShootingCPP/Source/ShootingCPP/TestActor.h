// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"				// 엔진을 구동하는데 필요한 최소한의 해더
#include "GameFramework/Actor.h"		// 액터 클래스를 사용하기 위한 해더
#include "TestActor.generated.h"		// 기타 자동 생성된 코드를 사용하기 위한 해더

UCLASS()	// 언리얼 엔진의 가비지 콜랙터가 관리할 수 있게 됨
class SHOOTINGCPP_API ATestActor : public AActor
{
	GENERATED_BODY()	// 클래스의 기본 코드를 생성하는 매크로
	
public:	
	// Sets default values for this actor's properties
	ATestActor();		// 생성자

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	// 게임이 시작되거나 액터가 생성될 때 호출되는 함수(가상함수를 오버라이드 함)

	int32 MyInt1 = 12;	// CPP 전용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MyInt2 = 34;	// (에디터 디테일 + 클래스 디폴트창)에서 수정 가능하게 함, 블루프린트에서 읽기/쓰기 가능하게 함

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MyInt3 = 56;	// 디폴트에서만 수정 가능하게 함, 블루프린트에서 읽기만 가능하게 함

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 MyInt4 = 78;	// 인스턴스(디테일창)에서만 수정 가능하게 함, 블루프린트에서 읽기만 가능하게 함

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MyInt5 = 90;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int32 MyInt6 = 23;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 MyInt7 = 45;

	void Test1(); // CPP 전용

	UFUNCTION(BlueprintCallable)	// 블루프린트에서 호출 가능하게 함
	void Test2();

	UFUNCTION(BlueprintPure)		// 블루프린트에서 호출 가능하게 함(순서가 상관 없어진다.)
	int32 Test3();

	// 블루프린트에서 구현 가능하게 함(함수 호출은 CPP에서 하고 싶은데 구현은 블루프린트에서 하고 싶을 때 사용)
	UFUNCTION(BlueprintImplementableEvent)	
	void Test4();

	// CPP에서 구현하고 블루프린트에서 오버라이드 가능하게 함(CPP에서 실행을 해도 블루프린트 구현 내용도 실행된다.)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Test5();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	// 매 프레임마다 호출되는 함수(가상함수를 오버라이드 함)

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))	// 매우 비추천(가능은 함)
	int32 MyInt8 = 67;		

};
