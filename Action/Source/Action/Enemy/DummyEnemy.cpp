// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "../UI/UserWidget_PopupDamage.h"

// Sets default values
ADummyEnemy::ADummyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ADummyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

float ADummyEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Damage : %f"), ActualDamage));

	

	return ActualDamage;
}

void ADummyEnemy::Die()
{
	Super::Die();

	DropItems();		// 아이템 드랍
	Destroy();			// 액터 삭제
}

// Called every frame
void ADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void ADummyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}