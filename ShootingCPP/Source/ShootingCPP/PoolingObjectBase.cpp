// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingObjectBase.h"

// Sets default values
APoolingObjectBase::APoolingObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}

void APoolingObjectBase::Activate()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);	// 부모(풀)로부터 분리
	OnActivate();						// 파생 클래스에서 구현한 활성화 함수 호출
}

void APoolingObjectBase::Deactivate()
{
	OnDeactivate();						// 파생 클래스에서 구현한 비활성화 함수 호출
	OnObjectDeactivate.Broadcast(this);	// 비활성화 되었다고 알림(델리게이트 호출)
}

void APoolingObjectBase::OnActivate()
{
	// 풀링 오브젝트가 활성화 되었을 때 파생 클래스별로 처리해야 할 일을 여기서 구현
}

void APoolingObjectBase::OnDeactivate()
{
	// 풀링 오브젝트가 비활성화 되었을 때 파생 클래스별로 처리해야 할 일을 여기서 구현
}