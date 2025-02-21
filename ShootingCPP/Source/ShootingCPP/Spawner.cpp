// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Components/ArrowComponent.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	AddSpawnerBaseComponent();
	AddSpawnerAreaComponent();
	AddSpawnerDiagonalComponent();
	AddSpawnerMassComponent();
}

void ASpawner::AddSpawnerBaseComponent()
{
	SpawnAreaBase = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Base"));	// 스폰될 영역 컴포넌트 생성
	SpawnAreaBase->SetupAttachment(RootComponent);
	SpawnAreaBase->SetCollisionProfileName(TEXT("NoCollision"));	// 충돌 없음

	// 스폰 영역의 forward 방향을 표시하기 위한 화살표 컴포넌트 생성
	UArrowComponent* ArrowBase = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Base")); 
	ArrowBase->SetupAttachment(SpawnAreaBase);
	ArrowBase->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerBaseType = CreateDefaultSubobject<USpawnerBaseComponent>(TEXT("Spawner_Base"));	// 스포너 컴포넌트 생성
	SpawnerBaseType->SetSpawnArea(SpawnAreaBase);
}

void ASpawner::AddSpawnerAreaComponent()
{
	SpawnAreaArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Area"));
	SpawnAreaArea->SetupAttachment(RootComponent);
	SpawnAreaArea->SetCollisionProfileName(TEXT("NoCollision"));	// 충돌 없음

	UArrowComponent* ArrowArea = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Area"));
	ArrowArea->SetupAttachment(SpawnAreaArea);
	ArrowArea->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerAreaType = CreateDefaultSubobject<USpawnerAreaComponent>(TEXT("Spawner_Area"));
	SpawnerAreaType->SetSpawnArea(SpawnAreaArea);
}

void ASpawner::AddSpawnerDiagonalComponent()
{
	SpawnAreaDiagonal = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Diagonal"));
	SpawnAreaDiagonal->SetupAttachment(RootComponent);
	SpawnAreaDiagonal->SetCollisionProfileName(TEXT("NoCollision"));	// 충돌 없음

	UArrowComponent* ArrowDiagonal = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Diagonal"));
	ArrowDiagonal->SetupAttachment(SpawnAreaDiagonal);
	ArrowDiagonal->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerDiagonalType = CreateDefaultSubobject<USpawnerDiagonalComponent>(TEXT("Spawner_Diagonal"));
	SpawnerDiagonalType->SetSpawnArea(SpawnAreaDiagonal);
}

void ASpawner::AddSpawnerMassComponent()
{
	SpawnAreaMass = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea_Mess"));
	SpawnAreaMass->SetupAttachment(RootComponent);
	SpawnAreaMass->SetCollisionProfileName(TEXT("NoCollision"));	// 충돌 없음

	UArrowComponent* ArrowMass = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_Mess"));
	ArrowMass->SetupAttachment(SpawnAreaMass);
	ArrowMass->SetRelativeRotation(FRotator::ZeroRotator);

	SpawnerMassType = CreateDefaultSubobject<USpawnerMassComponent>(TEXT("Spawner_Mess"));
	SpawnerMassType->SetSpawnArea(SpawnAreaMass);
}
