// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemyBase.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectileDamageTypeBase.h"
#include "Kismet/GameplayStatics.h"

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

void APoolingEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APoolingEnemyBase::OnEnemyActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APoolingEnemyBase::OnEnemyActorEndOverlap);
	OnTakeAnyDamage.AddDynamic(this, &APoolingEnemyBase::OnEnemyAnyDamage);
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

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(LifeTimeTimer);	// 타이머 해제
	LifeTimeTimer.Invalidate();				// 타이머 무효화

	ProjectileMovement->Velocity = FVector::ZeroVector;	// 이동 속도 초기화

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 비활성화
	StaticMesh->SetVisibility(false);									// 시각적으로 보이지 않게 함

	if (bIsInitialized)
	{
		// 첫번째 비활성화 이후에만 실행
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World, SelfExplosionEffect, 
			StaticMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(1.0f),
			true, true, ENCPoolMethod::AutoRelease);	// 터지는 이펙트 생성
	}
	else
	{
		bIsInitialized = true;	// 첫번째는 스킵하기 위한 변수
	}
}

void APoolingEnemyBase::Attack(AActor* Target)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Attack!") );
	if (Target)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Target : %s"), *Target->GetActorLabel()));
		// 플레이어에게 데미지를 줌
		UGameplayStatics::ApplyDamage(Target, DamageTickInterval * DamagePerSecond, nullptr, nullptr, nullptr);
	}
}

void APoolingEnemyBase::Die()
{
	// 점수 증가
	
	// 비활성화
	Deactivate();
}

void APoolingEnemyBase::OnEnemyAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	if (DamageType)	
	{		
		// 데미지 타입이 있으면 약점 속성에 따른 데미지 계산
		Health -= CalcFinalDamage(Damage, DamageType);	
	}
	else
	{
		Health -= Damage;	// 일반적인 데미지 계산
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, 
		FString::Printf(TEXT("Enemy Health : [%.1f/%.1f]"), Health, MaxHealth));

	// 처음 죽으면 Die 함수 호출
	if (Health <= 0.0f /*|| FMath::IsNearlyZero(Health)*/ )
	{
		Die();
	}
}

void APoolingEnemyBase::OnEnemyActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Overlap - Begin!"));
	if (OtherActor->ActorHasTag("Player") && !DamageTimer.IsValid())
	{
		// 플레이어와 겹치면 플레이어에게 데미지를 줌
		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.SetTimer(
			DamageTimer, 
			[this, OtherActor]()
			{
				Attack(OtherActor);
			},
			DamageTickInterval, true);
	}
}

void APoolingEnemyBase::OnEnemyActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Overlap - End!"));
	if (OtherActor->ActorHasTag("Player") && DamageTimer.IsValid())
	{
		// 플레이어가 범위에서 나가면 공격하는 타이머 해제
		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(DamageTimer);
		DamageTimer.Invalidate();
	}
}

float APoolingEnemyBase::CalcFinalDamage(float BaseDamage, const UDamageType* DamageType)
{
	float FinalDamage = BaseDamage;

	const UProjectileDamageTypeBase* ProjectileDamageType = Cast<UProjectileDamageTypeBase>(DamageType);
	// UDamageType이 UProjectileDamageTypeBase로 캐스팅 되면(UDamageType이 UProjectileDamageTypeBase의 파생 클래스)
	if (ProjectileDamageType && ProjectileDamageType->Type == WeakType)
	{
		FinalDamage *= 2.0f;	// 약점 속성이면 데미지 2배
	}
	
	return FinalDamage;
}
