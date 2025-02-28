// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManagerComponent.h"
#include "WeaponActor.h"
#include "../Player/ActionPlayerCharacter.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AActionPlayerCharacter>(GetOwner());

	WeaponInstances.Empty(WeaponClasses.Num());

	UWorld* World = GetWorld();
	FVector Offset = FVector(0.0f, 0.0f, -10000.0f);
	for (auto classType : WeaponClasses)
	{
		AWeaponActor* Weapon = World->SpawnActor<AWeaponActor>(
			classType, 
			Offset, FRotator::ZeroRotator);	// Offset위치에 무기 생성

		Weapon->AttachToComponent(
			OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::KeepWorldTransform,
			FName("roor"));					// 플레이어 아래에 무기를 추가
		Weapon->SetOwnerPlayer(OwnerCharacter);	// 무기에 플레이어를 소유자로 설정
		Weapon->WeaponActivate(false);		// 무기 비활성화

		WeaponInstances.Add(Weapon);
	}
	
}

