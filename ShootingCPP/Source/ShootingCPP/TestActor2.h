// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor2.generated.h"

UCLASS(NotBlueprintable)
class SHOOTINGCPP_API ATestActor2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 언리얼 기본 네이밍 규칙
	// 1. 변수명은 대문자로 시작
	// 2. 단어 사이는 대문자로 구분(_도 피할것)

	bool bBoolType = false;		// 부울
	int8 ByteType = 0;			// 바이트
	int32 IntType = 0;			// 인티저(32비트 정수)
	int64 Int64Type = 0;		// 인티저64(64비트 정수)
	float FloatType = 0.0f;		// 플로트(32비트)

	FName NameType = NAME_None;		// 네임(이름 표시용, 비교가 빠르다.)
	FString StringType = TEXT("");	// 스트링(일반적인 문자열)
	FText TextType = FText::FromString(TEXT("Hello World"));	// 텍스트(다국어 지원)
	FVector VectorType = FVector::ZeroVector;			// 벡터(3차원 좌표)
	FRotator RotatorType = FRotator::ZeroRotator;		// 로테이터(3차원 회전값)
	FTransform TransformType = FTransform::Identity;	// 트랜스폼(3차원 좌표와 회전값)

	UObject* ObjectType = nullptr;		// 오브젝트(모든 언리얼 오브젝트의 부모)
	AActor* ActorType = nullptr;		// 액터(모든 액터의 부모)
	UActorComponent* ActorComponentType = nullptr;		// 액터 컴포넌트(모든 액터 컴포넌트의 부모)

};
