// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	bIsAlive = true;
}

float AEnemyBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth -= ActualDamage;
	if (bIsAlive && CurrentHealth <= 0)
	{
		Die();
	}

	return ActualDamage;
}

void AEnemyBase::Die()
{
	bIsAlive = false;
	OnDie.Broadcast();
	Destroy();
}
