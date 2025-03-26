// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"
#include "Action/Weapon/EnemyWeaponActor.h"

ANormalEnemy::ANormalEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	ChildActorComponent->SetupAttachment(GetMesh(), TEXT("Weapon_H"));
	ChildActorComponent->SetChildActorClass(WeaponClass);

	UCharacterMovementComponent* EnemyMovement = GetCharacterMovement();
	EnemyMovement->MaxWalkSpeed = MaxWalkSpeed;
}

void ANormalEnemy::Attack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Normal Enemy Attack"));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Normal Enemy Attack"));

	if (AttackMontage && bIsAlive)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	Weapon = Cast<AEnemyWeaponActor>(ChildActorComponent->GetChildActor());
}
