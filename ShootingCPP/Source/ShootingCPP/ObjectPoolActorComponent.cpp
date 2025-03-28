// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolActorComponent.h"

// Sets default values for this component's properties
UObjectPoolActorComponent::UObjectPoolActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UObjectPoolActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Pool.Empty(InitialPoolSize);	// Pool 비우기(InitialPoolSize 크기로 초기화)
	if (PoolingObjectClass)			// PoolingObjectClass가 지정되어 있다면 생성 시작	
	{
		for (int32 i = 0; i < InitialPoolSize; i++)
		{
			SpawnNewPooledObject();	// InitialPoolSize만큼 생성
		}
	}	
}

APoolingObjectBase* UObjectPoolActorComponent::GetObject(const FTransform& Transform)
{
	APoolingObjectBase* PooledObject = nullptr;		// 반환할 오브젝트

	if (Pool.Num() > 0)	// Pool에 오브젝트가 있으면
	{
		PooledObject = Pool.Pop(EAllowShrinking::No);	// Pool에서 꺼내온다.(주의! - 리사이즈 테스트 필요)		
	}
	else
	{
		PooledObject = SpawnNewPooledObject();		// Pool이 비어있으면 새로 생성(만약을 대비한 코드)
	}

	if (PooledObject)
	{
		PooledObject->SetActorTransform(Transform);	// 트랜스폼 설정
		PooledObject->Activate();					// 활성화
	}

	if (Pool.Num() == 0)
	{
		SpawnNewPooledObject();		// Pool에서 마지막 오브젝트를 꺼냈으면 새로 생성
	}

	return PooledObject;
}

APoolingObjectBase* UObjectPoolActorComponent::GetObject()
{
	return GetObject(FTransform::Identity);	// GetObject(FTransform&) 호출
}

APoolingObjectBase* UObjectPoolActorComponent::SpawnNewPooledObject()
{
	AActor* Owner = GetOwner();	// 이 컴포넌트가 들어있는 액터를 가져오기
	if (!Owner)
	{
		return nullptr;			// 오너가 없으면 null을 리턴하고 종료
	}

	FActorSpawnParameters SpawnParams;	// 스폰 파라미터 생성
	SpawnParams.Owner = Owner;			// 스폰 파라미터의 오너를 Owner로 설정
	UWorld* World = GetWorld();			// 월드 가져오기
	APoolingObjectBase* NewPoolObject = World->SpawnActor<APoolingObjectBase>(
		PoolingObjectClass, 
		Owner->GetActorTransform(),		// 오너의 트랜스폼을 사용해서 배치
		SpawnParams
	);	// PoolingObjectClass를 생성

	if (NewPoolObject)
	{
		NewPoolObject->OnObjectDeactivate.AddDynamic(this, 
			&UObjectPoolActorComponent::SendToPool);	// 비활성화되면 SendToPool 함수 호출
		NewPoolObject->Deactivate();					// 생성된 오브젝트를 비활성화
	}

	return NewPoolObject;
}

void UObjectPoolActorComponent::SendToPool(APoolingObjectBase* Object)
{
	AActor* Owner = GetOwner();	// 이 컴포넌트가 들어있는 액터를 가져오기
	if (Owner && Object)
	{
		Object->AttachToActor(Owner, FAttachmentTransformRules::KeepWorldTransform);	// 오너에 붙이기
		Object->SetActorLocation(Owner->GetActorLocation() + LocationOffset);			// 위치 설정(오너위치 + LocationOffset)
	}
	Pool.Add(Object);			// Pool에 추가
}

