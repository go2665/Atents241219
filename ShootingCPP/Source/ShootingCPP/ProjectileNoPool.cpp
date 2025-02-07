// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileNoPool.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileNoPool::AProjectileNoPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트 생성
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);	// 루트 컴포넌트로 설정

	// 비주얼 이펙트 생성
	VisualEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VisualEffect->SetupAttachment(Root);

	// 구체 컴포넌트 생성
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); 
	Sphere->SetupAttachment(Root);
	Sphere->SetSphereRadius(8);		// 반지름 8로 설정
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	// 겹침만 체크(동적으로 겹침을 감지하는 콜리전 프로필로 설정)

	// 이동 컴포넌트 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 500.0f;			// 초기	속도 500
	ProjectileMovement->ProjectileGravityScale = 0.0f;	// 중력 적용 안함
	ProjectileMovement->Velocity = FVector(500, 0, 0);	// FVector::ForwardVector * 500;
	ProjectileMovement->bConstrainToPlane = true;		// 평면에 제약을 둠
	ProjectileMovement->ConstrainNormalToPlane(FVector(0, 0, 1));	// +Z축을 노멀로 하도록 제약을 둠
}

// Called when the game starts or when spawned
void AProjectileNoPool::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AProjectileNoPool::OnOverlapBegin);	// 이벤트 바인딩
	
	FTimerManager& TimerManager = GetWorldTimerManager();
	FTimerHandle LifeTimeTimerHandle;
	TimerManager.SetTimer(LifeTimeTimerHandle, this, &AProjectileNoPool::Explosion, LifeTime, false);	// 수명이 다되면 폭발하도록 타이머 설정
}

void AProjectileNoPool::Explosion()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Explosion"));
	if (ExplosionEffect != nullptr)
	{
		// 터지는 이펙트가 있으면 이펙트 생성
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), ExplosionEffect, 
			GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f),
			true, true, ENCPoolMethod::AutoRelease, true);		// AutoRelease로 자동으로 메모리 해제
	}

	Destroy();	// 폭발 후에는 총알을 제거
}

void AProjectileNoPool::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	//if (OverlappedActor != nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, 
	//		FString::Printf(TEXT("OverlappedActor : %s"), *(OverlappedActor->GetActorLabel())));	// 자신
	//}
	if (OtherActor != nullptr)	// 체크할 필요가 없을듯?
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow,
		//	FString::Printf(TEXT("OtherActor : %s"), *(OtherActor->GetActorLabel())));	// 상대방

		// 태그가 Player나 Bullet이 아닌 다른 액터와 겹칠 때
		if (!OtherActor->ActorHasTag(TEXT("Player")) && !OtherActor->ActorHasTag(TEXT("Bullet")))	
		{			
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, nullptr, nullptr);	// 상대에게 데미지 전달하기
			Explosion();	// 자신은 폭발
		}
	}
}
