// Fill out your copyright notice in the Description page of Project Settings.


#include "CellActor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "CellData.h"

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
	SensorCollision->SetBoxExtent(FVector(CellHalfSize * 0.75f, CellHalfSize * 0.75f, 200.0f));

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

void ACellActor::OnSensorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsClear && OtherActor->ActorHasTag("Player"))
	{
		CloseGate();
	}
}

void ACellActor::OpenGate()
{
	if (!bIsOpened)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OpenGate"));
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CloseGate"));
		bIsOpened = false;
		for (int i = 0; i < 4; i++)
		{
			GateMeshArray[i]->SetVisibility(true);
			GateMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}
