// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemyBase.h"

APoolingEnemyBase::APoolingEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	Tags.Add("Enemy");
}

void APoolingEnemyBase::OnActivate()
{
	Super::OnActivate();

	Health = MaxHealth;	// 체력 초기화
	ProjectileMovement->Velocity = MoveSpeed * GetActorForwardVector();	// 이동 속도 초기화
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);		// 충돌 활성화(알림만 보냄)
	StaticMesh->SetVisibility(true);									// 시각적으로 보이게 함

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(
		LifeTimeTimer, this, &APoolingEnemyBase::Deactivate, LifeTime, false);	// 수명이 다되면 비활성화
}

void APoolingEnemyBase::OnDeactivate()
{
	Super::OnDeactivate();
}

void APoolingEnemyBase::Attack()
{
}

void APoolingEnemyBase::OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
}

void APoolingEnemyBase::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

void APoolingEnemyBase::OnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

float APoolingEnemyBase::CalcFinalDamage(float BaseDamage, const UDamageType* DamageType)
{
	return 0.0f;
}
