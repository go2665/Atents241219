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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	// 매 프레임마다 호출되는 함수(가상함수를 오버라이드 함)

};
