// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerBaseComponent.h"
#include "GameModeCPP.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
USpawnerBaseComponent::USpawnerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USpawnerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGameModeCPP>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		FTimerHandle TempTimerHandle;
		TimerManager.SetTimer(
			TempTimerHandle,
			this, &USpawnerBaseComponent::Spawn,
			SpawnInterval, true);
	}
}

void USpawnerBaseComponent::OnRegister()
{
	Super::OnRegister();

	AddBoxAndArrowComponent();

	//AActor* Owner = GetOwner();
	//if (Owner)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *(Owner->GetName()));
	//}
}

void USpawnerBaseComponent::Spawn()
{
	FTransform SpawnTransform = GetSpawnTransform();
	GameMode->GetEnemy(SpawnEnemyType, SpawnTransform);
}

FTransform USpawnerBaseComponent::GetSpawnTransform() const
{
	return SpawnArea ? SpawnArea->GetComponentTransform() : FTransform::Identity;
}

void USpawnerBaseComponent::AddBoxAndArrowComponent()
{
	//AActor* Owner = GetOwner();
	//if (Owner && !SpawnArea)
	//{
	//	SpawnArea = NewObject<UBoxComponent>(Owner);
	//	if (SpawnArea)
	//	{
	//		SpawnArea->AttachToComponent(Owner->GetRootComponent(),
	//			FAttachmentTransformRules::KeepRelativeTransform);
	//		SpawnArea->SetRelativeLocation(FVector::ZeroVector);
	//		FString NewName = GetName() + TEXT("_SpawnArea");
	//		SpawnArea->Rename(*NewName);
	//		SpawnArea->RegisterComponent();

	//		UArrowComponent* ArrowComponent = NewObject<UArrowComponent>(Owner);
	//		ArrowComponent->AttachToComponent(SpawnArea,
	//			FAttachmentTransformRules::KeepRelativeTransform);
	//		ArrowComponent->SetRelativeLocation(FVector::ZeroVector);
	//		ArrowComponent->SetRelativeRotation(FRotator::ZeroRotator);
	//		ArrowComponent->RegisterComponent();
	//	}
	//}
}
