// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

APoolingProjectileBase::APoolingProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;	// 틱 끄기

	ProjectileVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVfx"));	// 에셋설정은 BP에서 처리
	ProjectileVfx->SetupAttachment(RootComponent);	

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetSphereRadius(8);										// 반지름이 8
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));		// 모든 다이나믹 오브젝트와 겹치기
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);		// 충돌 비활성화

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = InitialSpeed;								// 시작 속도 지정
	Movement->ProjectileGravityScale = 0.0f;							// 중력 비활성화	
	Movement->Velocity = FVector(InitialSpeed, 0, 0);					// 속도 지정
	Movement->bConstrainToPlane = true;									// 평면에 제약
	Movement->ConstrainNormalToPlane(FVector(0, 0, 1));					// 평면 법선(노멀) 지정

	this->Tags.Add(FName("Bullet"));	// Bullet으로 태그 추가
}

void APoolingProjectileBase::OnConstruction(const FTransform& Transform)
{
	if (ProjectileVfx)
	{
		// ProjectileVfx에 ProjectileColor 색상 설정
		ProjectileVfx->SetNiagaraVariableVec3(TEXT("ProjectileColor"), FVector(ProjectileColor.R, ProjectileColor.G, ProjectileColor.B));
	}
}

void APoolingProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(
		this, &APoolingProjectileBase::OnComponentBeginOverlap);	// 컴포넌트에 겹칠 때 호출할 함수 지정
}

void APoolingProjectileBase::OnActivate()
{
	ProjectileVfx->Activate(true);									// 나이아가라 컴포넌트 활성화
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 충돌 처리를 쿼리로만 처리(겹침 알림만 날림)
	Movement->Activate(true);										// 이동 컴포넌트 활성화
	Movement->SetVelocityInLocalSpace(FVector(InitialSpeed, 0, 0));	// 초기 속도 설정

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(	// 타이머 설정
		LifeTimeTimerHandle, 
		this, &APoolingProjectileBase::OnExplosion,
		LifeTime, false);	// 수명이 다되면 자동으로 터지기

	SetActorHiddenInGame(false);	// 게임에서 보이게 설정
}

void APoolingProjectileBase::OnDeactivate()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(LifeTimeTimerHandle);	// 타이머 해제
	LifeTimeTimerHandle.Invalidate();				// 핸들러 무효화
	
	ProjectileVfx->Deactivate();									// 나이아가라 컴포넌트 비활성화
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 비활성화
	Movement->Deactivate();											// 이동 컴포넌트 비활성화	

	SetActorHiddenInGame(true);
}

void APoolingProjectileBase::OnExplosion()
{
	if (ExplosionEffect)	// 이팩트가 있을 때만 처리
	{
		// 나이아가라 이팩트 생성
		UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), ExplosionEffect, 
			GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f),
			true, true, ENCPoolMethod::AutoRelease, true);

		SpawnedEffect->SetNiagaraVariableLinearColor(TEXT("BaseColor"), ProjectileColor);	// 색상 설정
	}

	Deactivate();	// 비활성화
}

void APoolingProjectileBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player") && !OtherActor->ActorHasTag("Bullet"))	// 플레이어와 총알이 아닌 경우
	{
		// 데미지 처리
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, nullptr, nullptr);

		// 터지는 이펙트 처리
		OnExplosion();
	}
}
