// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

APoolingProjectileBase::APoolingProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVfx"));	// 에셋설정은 BP에서 처리
	ProjectileVfx->SetupAttachment(RootComponent);	

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetSphereRadius(8);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = InitialSpeed;
	Movement->ProjectileGravityScale = 0.0f;
	Movement->Velocity = FVector(InitialSpeed, 0, 0);
	Movement->bConstrainToPlane = true;
	Movement->ConstrainNormalToPlane(FVector(0, 0, 1));

	this->Tags.Add(FName("Bullet"));
}

void APoolingProjectileBase::OnConstruction(const FTransform& Transform)
{
	if (ProjectileVfx)
	{
		ProjectileVfx->SetNiagaraVariableVec3(TEXT("ProjectileColor"), FVector(ProjectileColor.R, ProjectileColor.G, ProjectileColor.B));
	}
}

void APoolingProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(
		this, &APoolingProjectileBase::OnComponentBeginOverlap);
}

void APoolingProjectileBase::OnActivate()
{
	ProjectileVfx->Activate(true);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Movement->Activate(true);
	Movement->SetVelocityInLocalSpace(FVector(InitialSpeed, 0, 0));

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(
		LifeTimeTimerHandle, 
		this, &APoolingProjectileBase::OnExplosion,
		LifeTime, false);	// 수명이 다되면 자동으로 터지기

	SetActorHiddenInGame(false);
}

void APoolingProjectileBase::OnDeactivate()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(LifeTimeTimerHandle);	// 타이머 해제
	LifeTimeTimerHandle.Invalidate();				// 핸들러 무효화
	
	ProjectileVfx->Deactivate();
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Movement->Deactivate();

	SetActorHiddenInGame(true);
}

void APoolingProjectileBase::OnExplosion()
{
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), ExplosionEffect, 
		GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f),
		true, true, ENCPoolMethod::AutoRelease, true);

	SpawnedEffect->SetNiagaraVariableLinearColor(TEXT("BaseColor"), ProjectileColor);	

	Deactivate();
}

void APoolingProjectileBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player") && !OtherActor->ActorHasTag("Bullet"))
	{
		// 데미지 처리
		UGameplayStatics::ApplyDamage(OtherActor, Damage,
			nullptr, nullptr, nullptr);

		// 터지는 이펙트 처리
		OnExplosion();
	}
}
