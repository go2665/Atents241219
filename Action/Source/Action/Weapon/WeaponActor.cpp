// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/ActionPlayerCharacter.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(Root);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));	
}

void AWeaponActor::WeaponActivate(bool bActivate)
{	
	if (bActivate)
	{
		AttachToComponent(
			OwnerPlayer->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
		WeaponMesh->SetVisibility(true);
	}
	else
	{
		AttachToComponent(
			OwnerPlayer->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("root"));
		SetActorRelativeLocation(FVector(0.0f, 0.0f, -10000.0f));
		WeaponMesh->SetVisibility(false);	
	}
}

void AWeaponActor::SetCollisionActivate(bool bActivate)
{
	if (bActivate)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AWeaponActor::OnWeaponBeginOverlap);
}

void AWeaponActor::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != OwnerPlayer)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, 
			FString::Printf(TEXT("Weapon Overlap : %s"), *OtherActor->GetActorLabel()));*/
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, nullptr, nullptr);
	}
}

void AWeaponActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

