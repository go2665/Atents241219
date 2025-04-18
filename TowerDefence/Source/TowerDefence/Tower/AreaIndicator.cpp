// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaIndicator.h"

// Sets default values
AAreaIndicator::AAreaIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 만들기
	// 스태틱 메시 만들기
	// 스태틱 메시가 루트의 자식이 되기
	//AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMesh"));
}

// Called when the game starts or when spawned
void AAreaIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAreaIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 마우스 위치에 따라 AreaIndicator 위치를 업데이트(높이는 +1)

}

