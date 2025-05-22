// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Enemy/Enemy.h"
#include "TowerDefence/Tower/Data/ShotDataAsset.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bRotationRemainsVertical = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetEnemy) // TargetActor가 없으면 범위공격이다.
	{
		if (TargetEnemy->IsAlive())
		{
			// 목표가 살아있으면 목표 방향으로 계속 회전
			FVector Direction = TargetEnemy->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
			SetActorRotation(Direction.Rotation()); // 발사체가 날아가는 방향으로 회전
			ProjectileMovement->Velocity = Direction * MoveSpeed;		
		}
	}
	else
	{
		// 목표 위치로 날아가다가 목표 위치에 도착하면 DamageToArea 호출
		FVector Direction = TargetLocation - GetActorLocation();
		float DistSquared = Direction.SizeSquared();
		//UE_LOG(LogTemp, Warning, TEXT("DistSquared: %.2f"), DistSquared);
		if (DistSquared < 100)	// 목표 위치와 10cm 안인지 확인
		{
			if (bShowDebugInfo)
			{
				UWorld* World = GetWorld();
				float Radius = ShotData->bIsSplashAttack ? GetShotLevelData().SplashRadius : 10.0f;
				DrawDebugSphere(
					World,
					TargetLocation,
					Radius,
					12,
					FColor::Red,
					false,
					1.0f
				);
			}

			DamageToArea(nullptr);	// 바닥에 충돌
			
			GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReleaseObject(this); // 발사체 풀로 반환
		}
	}
}

void AProjectile::OnSpawn(const FTransform& InTransform, const UShotDataAsset* InShotData, const AEnemy* InTarget, int32 InLevel, float InDamage, bool InbShowDebugInfo)
{
	//UE_LOG(LogTemp, Warning, TEXT("Projectile[%s]: OnInitialize"), *this->GetName());

	// 타겟과 발사체 데이터는 반드시 존재해야 한다.
	verify(InTarget != nullptr && InShotData != nullptr);

	SetActorTransform(InTransform); // 초기 트랜스폼 설정

	TargetEnemy = InTarget;
	TargetLocation = TargetEnemy->GetActorLocation();

	FVector Direction = TargetEnemy->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	ProjectileMovement->Velocity = Direction * MoveSpeed;	

	// 샷 데이터 설정
	ShotData = InShotData;
	if (ShotData->bIsSplashAttack)
	{
		TargetEnemy = nullptr; // 범위 공격이면 TargetActor는 nullptr로 설정
	}

	ShotLevel = InLevel;

	Damage = InDamage;

	// 이미 겹쳐있는 적 처리
	TArray<AActor*> OverlapActors;
	GetOverlappingActors(OverlapActors);
	for (AActor* Actor : OverlapActors)
	{
		OnOverlapEnemy(this, Actor);
	}

	bShowDebugInfo = InbShowDebugInfo;	// 디버그 정보 표시 여부
	if (bShowDebugInfo)
	{
		UWorld* World = GetWorld();
		DrawDebugLine(
			World,
			GetActorLocation(),
			TargetLocation,
			FColor::Yellow,
			false,
			1.0f,
			0,
			1.0f
		);
		float Radius = ShotData->bIsSplashAttack ? GetShotLevelData().SplashRadius : 10.0f;
		DrawDebugSphere(
			World,
			TargetLocation,
			Radius,
			12,
			FColor::Yellow,
			false,
			1.0f
		);
	}
}

void AProjectile::OnOverlapEnemy(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Projectile[%s]: Overlapped with %s"), *this->GetName(), *OtherActor->GetName());
	if (OtherActor->ActorHasTag(FName("Enemy")))
	{
		OnHitEnemy(Cast<AEnemy>(OtherActor)); // 적과의 충돌 처리		
	}	
}

void AProjectile::DamageToEnemy(AEnemy* HitEnemy)
{
	if (ShotData)
	{
		// 적 한명에게 데미지 적용
		UGameplayStatics::ApplyDamage(
			HitEnemy,
			Damage,
			nullptr, this,
			ShotData->DamageType);

		// 맞은 적에게 이팩트 주기
		HitEnemy->AddEffect(GetShotLevelData().EffectType, ShotLevel);
	}
}

void AProjectile::DamageToArea(AActor* InIgnore)
{
	if (ShotData)
	{
		// 범위 공격 데미지 처리
		TArray<AActor*> Ignores;
		if (InIgnore)
			Ignores.Add(InIgnore); // InIgnore는 무시한다. 

		HitEnemies.Empty(); // 범위로 데미지 주기 전에 초기화

		UGameplayStatics::ApplyRadialDamageWithFalloff(
			GetWorld(),
			Damage,
			0.0f,
			GetActorLocation(),
			GetShotLevelData().SplashRadius * 0.5f,
			GetShotLevelData().SplashRadius,
			ShotData->SplashFalloff,
			ShotData->DamageType,
			Ignores,
			this // 이 발사체를 맞은 대상을 기록하기 위해 사용
		);

		// 이팩트 처리(HitEnemies에 들어있는 적 사용)
		if (GetShotLevelData().EffectType != EEffectType::None)
		{
			for (AEnemy* Target : HitEnemies)
			{
				AEnemy* Enemy = Cast<AEnemy>(Target);
				if (Enemy)
				{
					Enemy->AddEffect(GetShotLevelData().EffectType, ShotLevel);
				}
			}
		}
	}
}

const FShotLevelData& AProjectile::GetShotLevelData() const
{
	return ShotData->LevelData[ShotLevel];
}

void AProjectile::OnHitEnemy(AEnemy* InHitEnemy)
{
	if (InHitEnemy && bActivate)
	{
		bActivate = false;
		UE_LOG(LogTemp, Warning, TEXT("[%s] Hit [%s]"), 
			*this->GetActorNameOrLabel(), *InHitEnemy->GetActorNameOrLabel());
		if (bShowDebugInfo)
		{
			UWorld* World = GetWorld();
			float Radius = ShotData->bIsSplashAttack ? GetShotLevelData().SplashRadius : 10.0f;
			DrawDebugSphere(
				World,
				this->GetActorLocation(),
				Radius,
				12,
				FColor::Red,
				false,
				1.0f
			);

			int Minutes = FMath::FloorToInt(World->TimeSeconds / 60);
			float Seconds = FMath::Fmod(World->TimeSeconds, 60.0f);
			FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
			UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] : Hit!"), *TimeString, *this->GetActorNameOrLabel());
		}

		DamageToEnemy(InHitEnemy);		// 데미지 주기

		if (TargetEnemy == nullptr)
		{
			DamageToArea(InHitEnemy);	// 범위 공격에서는 HitEnemy를 이미 데미지를 줬으니 무시
		}
		
		GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReleaseObject(this); // 발사체 삭제
	}
}

void AProjectile::OnInitialize()
{
	ProjectileMovement->InitialSpeed = MoveSpeed;

	// 오버랩 이벤트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapEnemy);	
}

void AProjectile::OnActivate()
{
	// 10초 후에 발사체를 풀로 되돌린다.
	UWorld* World = GetWorld();
	TWeakObjectPtr<AProjectile> WeakThis(this);

	World->GetTimerManager().SetTimer(
		LifeTimerHandle,
		[WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				UObjectPoolSubsystem* PoolSubSystem = WeakThis->GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
				if (PoolSubSystem)
				{
					PoolSubSystem->ReleaseObject(WeakThis.Get());
				}
			}
		},
		10.0f, false);
	ProjectileMovement->SetActive(true);
	bActivate = true;

	int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
	float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
	FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
	UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] - OnActivate"), *TimeString, *this->GetName());
}

void AProjectile::OnDeactivate()
{
	int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
	float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
	FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
	UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] - OnDeactivate"), *TimeString, *this->GetName());

	ProjectileMovement->SetActive(false);
	if (LifeTimerHandle.IsValid())
	{
		UWorld* World = GetWorld();
		World->GetTimerManager().ClearTimer(LifeTimerHandle);
	}
	bActivate = false;
}
