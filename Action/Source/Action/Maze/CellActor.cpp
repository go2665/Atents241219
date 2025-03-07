// Fill out your copyright notice in the Description page of Project Settings.


#include "CellActor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "CellData.h"
#include "../Enemy/EnemyBase.h"

// Sets default values
ACellActor::ACellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundMesh"));
	BackgroundMesh->SetupAttachment(RootComponent);

	UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateNorth"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetRelativeLocation(FVector::ForwardVector * (CellHalfSize - GateHalfThickness));
	GateMeshArray.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateEast"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetRelativeLocation(FVector::RightVector * (CellHalfSize - GateHalfThickness));
	Gate->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	GateMeshArray.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateSouth"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetRelativeLocation(FVector::BackwardVector * (CellHalfSize - GateHalfThickness));
	Gate->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	GateMeshArray.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateWest"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetRelativeLocation(FVector::LeftVector * (CellHalfSize - GateHalfThickness));
	Gate->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GateMeshArray.Add(Gate);

	SensorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SensorCollision"));
	SensorCollision->SetupAttachment(RootComponent);
	SensorCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SensorCollision->SetBoxExtent(FVector(CellHalfSize * 0.75f, CellHalfSize * 0.75f, 100.0f));
	SensorCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
}

void ACellActor::Initialize(CellData* InCellData)
{
	if (InCellData)
	{
		Path = InCellData->GetPath();
		OpenGate();
	}
}

void ACellActor::BeginPlay()
{
	Super::BeginPlay();
	SensorCollision->OnComponentBeginOverlap.AddDynamic(this, &ACellActor::OnSensorBeginOverlap);
}

#if WITH_EDITOR
void ACellActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? 
		PropertyChangedEvent.Property->GetFName() : NAME_None;	// 변경된 프로퍼티의 이름 가져오기

	// EnemyCountMin나 EnemyCountMax가 변경되었을 때
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACellActor, EnemyCountMin) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ACellActor, EnemyCountMax))
	{
		// EnemyCountMax가 무조건 Min보다 크거나 같도록 설정
		EnemyCountMax = FMath::Max(EnemyCountMin, EnemyCountMax);
	}
}
#endif

void ACellActor::OnSensorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsClear && OtherActor->ActorHasTag("Player"))
	{
		CloseGate();
		SpawnEnemy();
	}
}

void ACellActor::OnEnemyDie()
{
	SpawnCount--;
	if (SpawnCount <= 0)
	{
		bIsClear = true;
		OpenGate();
	}
}

void ACellActor::OpenGate()
{
	if (!bIsOpened)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OpenGate"));
		bIsOpened = true;
		for (int i = 0; i < 4; i++)
		{
			if ((Path & static_cast<EDirectionType>(1 << i)) != EDirectionType::None)
			{
				GateMeshArray[i]->SetVisibility(false);
				GateMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ACellActor::CloseGate()
{
	if (bIsOpened)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CloseGate"));
		bIsOpened = false;
		for (int i = 0; i < 4; i++)
		{
			GateMeshArray[i]->SetVisibility(true);
			GateMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ACellActor::SpawnEnemy()
{
	if (EnemyClass)
	{
		UWorld* World = GetWorld();
		SpawnCount = FMath::RandRange(EnemyCountMin, EnemyCountMax);

		for (int i = 0; i < SpawnCount; i++)
		{
			FVector Location = FMath::RandPointInBox(SensorCollision->Bounds.GetBox());
			Location.Z = 0.0f;

			AEnemyBase* Enemy = World->SpawnActor<AEnemyBase>(EnemyClass, Location, FRotator::ZeroRotator);
			Enemy->OnDie.AddDynamic(this, &ACellActor::OnEnemyDie);
		}
	}
}
