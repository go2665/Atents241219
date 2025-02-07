// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolActorComponent.h"

// Sets default values for this component's properties
UObjectPoolActorComponent::UObjectPoolActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	/*
	* 한 종류의 APoolingObjectBase를 맴버 변수로 가진다.
	* 시작할 때 정해진 PoolSize만큼 APoolingObjectBase를 생성하여 Pool(배열)에 넣어둔다.
	* GetObject(FTransform&) 함수를 통해 Pool에서 APoolingObjectBase를 꺼내온다.
	* Pool에 APoolingObjectBase가 없다면 새로 생성하여 반환한다.
	* GetObject로 꺼낼 때 활성화 시킨다.(Activate실행. 오너의 자식에서 빠진다.)
	* Pool로 돌아갈 때 비활성화 시킨다.(Deactivate실행. 오너의 자식이 된다.)
	* APoolingObjectBase가 생성될 때는 자동으로 비활성화 된다.
	*/
}


// Called when the game starts
void UObjectPoolActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UObjectPoolActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

