// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Enemy/EnemyBase.h"
#include "TowerDefence/Tower/Data/ShotDataAsset.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("OverlapEnemyOnly"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bRotationRemainsVertical = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	verify(ShotData != nullptr);		// 발사체 데이터가 반드시 설정되어 있어야 함

	// 오버랩 이벤트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapEnemy);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor) // TargetActor가 없으면 범위공격이다.
	{
		// 목표 액터를 계속 따라가다가 부딪히면 DamageToEnemy 호출
		FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SetActorRotation(Direction.Rotation()); // 발사체가 날아가는 방향으로 회전
		ProjectileMovement->Velocity = Direction * MoveSpeed;		
	}
	else
	{
		// 목표 위치로 날아가다가 목표 위치에 도착하면 DamageToArea 호출
		FVector Direction = TargetLocation - GetActorLocation();
		float DistSquared = Direction.SizeSquared();
		//UE_LOG(LogTemp, Warning, TEXT("DistSquared: %.2f"), DistSquared);
		if (DistSquared < 100)	// 목표 위치와 10cm 안인지 확인
		{
			DamageToArea(nullptr);	// 바닥에 충돌
			Destroy();
		}
	}
}

void AProjectile::OnInitialize(const AActor* InTarget, const UShotDataAsset* InShotData,
	float InDamageModifier, float InEffectModifier)
{
	TargetActor = InTarget;
	TargetLocation = TargetActor->GetActorLocation();

	FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	ProjectileMovement->Velocity = Direction * MoveSpeed;
	ProjectileMovement->InitialSpeed = MoveSpeed;

	ShotData = InShotData;	
	if (ShotData->bIsAreaAttack)
	{
		TargetActor = nullptr; // 범위 공격이면 TargetActor는 nullptr로 설정
	}
	DamageModifier = InDamageModifier;
	EffectModifier = InEffectModifier;
}

void AProjectile::OnOverlapEnemy(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Enemy")))
	{
		AEnemyBase* HitEnemy = Cast<AEnemyBase>(OtherActor);
		if (HitEnemy)
		{
			DamageToEnemy(HitEnemy);	// 데미지 주기

			if (TargetActor == nullptr) 
			{
				DamageToArea(HitEnemy); // 범위 공격에서는 HitEnemy를 이미 데미지를 줬으니 무시
			}
			Destroy();					// 발사체 삭제
		}
	}
}

void AProjectile::DamageToEnemy(AEnemyBase* HitEnemy)
{
	// 적 한명에게 데미지 적용
	UGameplayStatics::ApplyDamage(
		HitEnemy,
		ShotData->Damage * DamageModifier,
		nullptr, nullptr,
		ShotData->AttributeType);

	// 적에게 디버프 추가
	//HitEnemy->GetDebuffComponent()->AddDebuff(ShotData->DebuffType, DebuffModifier);
}

void AProjectile::DamageToArea(AActor* InIgnore)
{
	// 범위 공격 데미지 처리

	TArray<AActor*> Ignores;
	if (InIgnore)
		Ignores.Add(InIgnore); // InIgnore는 무시한다. 

	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		ShotData->Damage * DamageModifier,
		0.0f,
		GetActorLocation(),
		ShotData->AreaRadius * 0.5f,
		ShotData->AreaRadius,
		ShotData->AreaFalloff,
		ShotData->AttributeType,
		Ignores,
		this // 이 발사체를 맞은 대상을 기록하기 위해 사용
	);

	// 디버프 처리
	//for (AActor* Target : HitEnemies)
	//{
	//	if (ShotData->DebuffType != EDebuffType::None)
	//	{
	//		AEnemyBase* Enemy = Cast<AEnemyBase>(Target);
	//		if (Enemy)
	//		{
	//			Enemy->GetDebuffComponent()->AddDebuff(ShotData->DebuffType, DebuffModifier); // 디버프 추가
	//		}
	//	}
	//}
}

