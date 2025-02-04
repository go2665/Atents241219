// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor_AddComponent.h"

// Sets default values
ATestActor_AddComponent::ATestActor_AddComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 단축키 : Ctrl + K + O (CPP와 H파일 전환)

	// 컴포넌트는 생성자에서 추가해야 한다.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));	// USceneComponent를 SceneRoot라는 이름으로 생성하고 Root에 저장
	SetRootComponent(Root);				// Root를 루트 컴포넌트로 설정

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	// UStaticMeshComponent를 Mesh라는 이름으로 생성
	StaticMesh->SetupAttachment(Root);	// StaticMesh를 Root에 부착
}

// Called when the game starts or when spawned
void ATestActor_AddComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor_AddComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

