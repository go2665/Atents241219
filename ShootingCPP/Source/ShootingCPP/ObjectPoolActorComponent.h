// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolingObjectBase.h"
#include "ObjectPoolActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGCPP_API UObjectPoolActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// 풀에서 오브젝트를 가져오는 함수(트랜스폼을 지정하여 오브젝트를 가져올 수 있다, 블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	APoolingObjectBase* GetObject(const FTransform& Transform);

	// 풀에서 오브젝트를 가져오는 함수(C++ 전용. 트랜스폼은 Identity를 사용.)
	APoolingObjectBase* GetObject();

protected:
	// 풀에 오브젝트가 없을 때 새로 생성하는 함수
	APoolingObjectBase* SpawnNewPooledObject();

	// 오브젝트를 풀 액터에 어태치하고 풀로 되돌리는 함수. 델리게이트에 연결을 위해 UFUNCTION() 추가
	UFUNCTION()
	void SendToPool(APoolingObjectBase* Object);

public:
	// 풀링 오브젝트의 클래스를 지정(APoolingObjectBase의 파생 클래스를 저장한다.)
	UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
	TSubclassOf<APoolingObjectBase> PoolingObjectClass = nullptr;

	// 풀의 초기 크기
	UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
	int32 InitialPoolSize = 5;

protected:
	// 풀링 되는 오브젝트들의 배열(전부 비활성화 된 오브젝트만 들어있다.)
	TArray<APoolingObjectBase*> Pool;

	// 풀에서 비활성화된 오브젝트의 위치 오프셋 
	UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
	FVector LocationOffset = FVector(0.f, 0.f, 10000.f);
};
