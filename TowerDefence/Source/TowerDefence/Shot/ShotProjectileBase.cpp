// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Enemy/EnemyBase.h"
#include "TowerDefence/Shot/Debuff/DebuffComponent.h"

// Sets default values
AShotProjectileBase::AShotProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

void AShotProjectileBase::OnConstruction(const FTransform& Transform)
{
	if (ShotData)
	{
		Mesh->SetStaticMesh(ShotData->ProjectileMesh);
		ProjectileMovement->InitialSpeed = ShotData->MoveSpeed;
	}
}

void AShotProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShotData->bIsAreaAttack) // 범위 공격은 무조건 바닥에 떨어짐
	{
		// 목표 위치로 날아가다가 목표 위치에 도착하면 OnHitEnemy 호출
		FVector Direction = TargetLocation - GetActorLocation();
		float DistSquared = Direction.SizeSquared();
		//UE_LOG(LogTemp, Warning, TEXT("DistSquared: %.2f"), DistSquared);
		if (DistSquared < 100)	// 목표 위치와 10cm 안인지 확인
		{
			OnHitEnemy(nullptr);	// 바닥에 충돌
		}
		//else
		//{
		//	Direction.Normalize();
		//	ProjectileMovement->Velocity = Direction * MoveSpeed;
		//} // 속도가 빠른데 Sweep이 잘되면 필요없음.		
	}
	else
	{
		// 목표 액터를 계속 따라가다가 부딪히면 OnHitEnemy 호출
		FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SetActorRotation(Direction.Rotation()); // 발사체가 날아가는 방향으로 회전
		ProjectileMovement->Velocity = Direction * ShotData->MoveSpeed;
	}
}

void AShotProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	check(ShotData != nullptr);		// 발사체 데이터가 반드시 설정되어 있어야 함
	
	// 오버랩 이벤트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AShotProjectileBase::OnOverlapEnemy);
}

void AShotProjectileBase::InitializeShotData(AActor* Target, UProjectileShotDataAsset* NewShotData)
{
	TargetActor = Target;
	TargetLocation = TargetActor->GetActorLocation();

	FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	ProjectileMovement->Velocity = Direction * ShotData->MoveSpeed;

	ShotData = NewShotData;
	Mesh->SetStaticMesh(ShotData->ProjectileMesh);
	ProjectileMovement->InitialSpeed = ShotData->MoveSpeed;
}

void AShotProjectileBase::OnOverlapEnemy(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Enemy")))
	{
		AEnemyBase* HitEnemy = Cast<AEnemyBase>(OtherActor);
		if (HitEnemy)
		{
			OnHitEnemy(HitEnemy);
		}
	}
}

void AShotProjectileBase::OnHitEnemy(AEnemyBase* HitEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHitEnemy : %s"), *GetNameSafe(HitEnemy));

	if (HitEnemy)
	{
		// 적과 충돌했을 때의 처리(적과 충돌했다는 것은 무조건 범위 공격이 아니다)
		// 적 한명에게 데미지 적용
		UGameplayStatics::ApplyDamage(
			HitEnemy, 
			ShotData->Damage, 
			nullptr, nullptr, 
			ShotData->AttributeType);

		HitEnemy->GetDebuffComponent()->AddDebuff(ShotData->DebuffType); // 디버프 추가		
	}

	if (ShotData->bIsAreaAttack)
	{
		// 적과 충돌하지 않았다(= 바닥에 충돌했다 = 범위 공격)

		HitEnemies.Empty(); // HitEnemies 배열 초기화(직접 충돌한 적은 제외)

		TArray<AActor*> Ignores;
		if (HitEnemy)
			Ignores.Add(HitEnemy); // HitEnemy는 무시한다. (자기 자신은 이미 데미지를 주었으니까 무시해야 함)

		UGameplayStatics::ApplyRadialDamageWithFalloff(
			GetWorld(),
			ShotData->Damage,
			0.0f,
			GetActorLocation(),
			ShotData->AreaRadius * 0.5f,
			ShotData->AreaRadius,
			ShotData->AreaFalloff,
			ShotData->AttributeType,
			Ignores,
			this // 이 발사체를 맞은 대상을 기록하기 위해 사용
		);

		for (AActor* Target : HitEnemies)
		{
			if (ShotData->DebuffType != EDebuffType::None)
			{
				AEnemyBase* Enemy = Cast<AEnemyBase>(Target);
				if (Enemy)
				{
					Enemy->GetDebuffComponent()->AddDebuff(ShotData->DebuffType); // 디버프 추가
				}
			}
		}
	}

	Destroy(); // 발사체 삭제
}
