// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyWeaponActor::AEnemyWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);	
	WeaponMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AEnemyWeaponActor::SetActivate(bool bActivate)
{
	if (bActivate)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemyWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnActorBeginOverlap.AddDynamic(this, &AEnemyWeaponActor::OnWeaponOverlapBegin);
}

void AEnemyWeaponActor::OnWeaponOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Overlap"));

		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, nullptr, nullptr);
	}
}


