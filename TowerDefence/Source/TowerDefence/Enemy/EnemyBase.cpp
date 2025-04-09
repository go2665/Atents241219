// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 설정
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 메쉬 컴포넌트 생성 및 설정
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(Root);
	EnemyMesh->SetCollisionProfileName(TEXT("Enemy")); // 적 캐릭터의 충돌 프로파일 설정

	// 태그 설정
	Tags.Add(FName("Enemy")); // 태그 추가
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

