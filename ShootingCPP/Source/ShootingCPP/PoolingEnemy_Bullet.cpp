// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

APoolingEnemy_Bullet::APoolingEnemy_Bullet()
{
	PrimaryActorTick.bCanEverTick = false;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	// 오버랩만 사용

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;	// 중력 적용 안함
	ProjectileMovement->bConstrainToPlane = true;		// XY평면에 제한
	ProjectileMovement->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));	// Z축이 노멀벡터

	Tags.Add(FName("Bullet"));	// 태그 추가
}

void APoolingEnemy_Bullet::BeginPlay()
{
	Super::BeginPlay();

	// 오버랩 이벤트 때 실행될 함수 설정
	OnActorBeginOverlap.AddDynamic(this, &APoolingEnemy_Bullet::OnBulletOverlapBegin);
}

void APoolingEnemy_Bullet::OnActivate()
{
	Super::OnActivate();

	ProjectileMovement->Velocity = MoveSpeed * GetActorForwardVector();	// 앞으로 이동하게 설정
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);		// 충돌 활성화
	BulletMesh->SetVisibility(true);									// 보이게 설정

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		LifeTimeTimer,
		this, &APoolingEnemy_Bullet::Deactivate,
		LifeTime, false);	// 수명이 다되면 Deactivate
}

void APoolingEnemy_Bullet::OnDeactivate()
{
	Super::OnDeactivate();

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearTimer(LifeTimeTimer);	// 타이머 정리
	LifeTimeTimer.Invalidate();				// 타이머 무효화

	ProjectileMovement->Velocity = FVector::ZeroVector;		// 이동 중지

	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 비활성화
	BulletMesh->SetVisibility(false);									// 보이지 않게 설정

	if (bIsInitialized)
	{
		// 첫번째 Deactivate는 제외하고 그 이후부터 폭발 이팩트 생성
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SelfExplosionEffect,
			BulletMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(1.0f),
			true, true, ENCPoolMethod::AutoRelease, true);
	}
	else
	{
		// 첫번째는 Pool에 들어가는 거니까 폭발 이팩트는 생성하지 않음
		bIsInitialized = true;
	}
}

void APoolingEnemy_Bullet::OnBulletOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))	// 플레이어와 겹친 경우
	{
		UGameplayStatics::ApplyDamage(
			OtherActor, Damage, nullptr, nullptr, nullptr);	// 데미지 적용하고
		Deactivate();						// 비활성화
	}
}
