// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDelegateFrom.h"

// Sets default values
ATestDelegateFrom::ATestDelegateFrom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestDelegateFrom::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATestDelegateFrom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestDelegateFrom::TestRunDelegate()
{
	OnFinish.Broadcast();	// 이벤트 델리게이트 호출. 연결된 리스너에게 이벤트 전달. (디스패치 실행과 같음)
}

