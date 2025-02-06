// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileNoPool.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileNoPool::AProjectileNoPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	VisualEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VisualEffect->SetupAttachment(Root);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); 
	Sphere->SetupAttachment(Root);
	Sphere->SetSphereRadius(8);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->Velocity = FVector(500, 0, 0);	// FVector::ForwardVector * 500;
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->ConstrainNormalToPlane(FVector(0, 0, 1));

}

// Called when the game starts or when spawned
void AProjectileNoPool::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerManager& TimerManager = GetWorldTimerManager();
	FTimerHandle LifeTimeTimerHandle;
	TimerManager.SetTimer(LifeTimeTimerHandle, this, &AProjectileNoPool::Explosion, LifeTime, false);
}

void AProjectileNoPool::Explosion()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Explosion"));
}
