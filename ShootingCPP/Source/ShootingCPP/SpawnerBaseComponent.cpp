// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerBaseComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
USpawnerBaseComponent::USpawnerBaseComponent()
{
	// 틱 끄기
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USpawnerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// 게임모드를 캐스팅 해 놓음(캐싱)
	GameMode = Cast<AGameModeCPP>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		// GameMode가 있다면 스폰 주기마다 Spawn 함수를 호출하도록 타이머 설정
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		FTimerHandle TempTimerHandle;
		TimerManager.SetTimer(
			TempTimerHandle,
			this, &USpawnerBaseComponent::Spawn,
			SpawnInterval, true);
	}
}

//void USpawnerBaseComponent::OnRegister()
//{
//	Super::OnRegister();
//
//	AddBoxAndArrowComponent();
//
//	AActor* Owner = GetOwner();
//	if (Owner)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("%s"), *(Owner->GetName()));
//	}
//}

void USpawnerBaseComponent::Spawn()
{	
	FTransform SpawnTransform = GetSpawnTransform();	// 스폰할 트랜스폼 가져오기
	GameMode->GetEnemy(SpawnEnemyType, SpawnTransform);	// 적 하나 스폰
}

FTransform USpawnerBaseComponent::GetSpawnTransform() const
{
	// 스폰 영역의 위치 가져오기(없으면 Identity)
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
