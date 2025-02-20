// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingProjectile_Area.h"
#include "Kismet/GameplayStatics.h"

APoolingProjectile_Area::APoolingProjectile_Area()
{
	AreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaCollision"));
	AreaCollision->SetupAttachment(RootComponent);
	AreaCollision->SetSphereRadius(50);
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AreaVfx"));
	AreaVfx->SetupAttachment(RootComponent);
	AreaVfx->SetAutoActivate(false);
}

void APoolingProjectile_Area::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (AreaVfx)
	{
		// AreaVfx에 ProjectileColor 색상 설정
		AreaVfx->SetVariableVec3(TEXT("BaseColor"), FVector(ProjectileColor.R, ProjectileColor.G, ProjectileColor.B));
	}
}

void APoolingProjectile_Area::BeginPlay()
{
	Super::BeginPlay();
	AreaCollision->OnComponentBeginOverlap.AddDynamic(
		this, &APoolingProjectile_Area::OnSecondAreaBeginOverlap);
	AreaCollision->OnComponentEndOverlap.AddDynamic(
		this, &APoolingProjectile_Area::OnSecondAreaEndOverlap);
}

void APoolingProjectile_Area::OnActivate()
{
	Super::OnActivate();

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(LifeTimeTimerHandle);	// 기존에 OnExplosion 실행하던 타이머 제거
	LifeTimeTimerHandle.Invalidate();

	TimerManager.SetTimer(
		LifeTimeTimerHandle,
		this, &APoolingProjectile_Area::SecondExplosion,
		LifeTime, false);	// 수명이 다되면 2차 폭발
}

void APoolingProjectile_Area::OnDeactivate()
{
	Super::OnDeactivate();

	AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaVfx->Deactivate();
	SecondTargets.Empty();

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White,
	//	FString::Printf(TEXT("OnDeactivate : %.1f"), GetWorld()->GetTimeSeconds()));
}

void APoolingProjectile_Area::OnExplosion()
{
	SecondExplosion();	// 어딘가에 부딪히면 2차 폭발
}

void APoolingProjectile_Area::SecondExplosion()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, 
	//	FString::Printf(TEXT("SecondExplosion : %.1f"), GetWorld()->GetTimeSeconds()));

	// 라이프타임 타이머 취소
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(LifeTimeTimerHandle);	
	LifeTimeTimerHandle.Invalidate();

	// 부모의 컴포넌트들 비활성화
	ProjectileVfx->Deactivate();
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Movement->Deactivate();

	// 자기 컴포넌트들 활성화
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaVfx->Activate(true);

	// 시작하자 마자 첫번째 2차 폭발 데미지 적용
	//ApplyDamageToSecondTargets();		// 블루프린트는 타이머가 무조건 선딜레이가 있음. CPP는 선딜레이 없음

	// 2차 폭발 지속 시간 동안 데미지 적용
	TimerManager.SetTimer(
		SecondExplosionTimerHandle,
		this, &APoolingProjectile_Area::ApplyDamageToSecondTargets,
		SecondTickInterval, true);

	// 2차 폭발 지속 시간 후에 끝내기
	TimerManager.SetTimer(
		LifeTimeTimerHandle,
		[this,&TimerManager]() {
			TimerManager.ClearTimer(SecondExplosionTimerHandle);
			SecondExplosionTimerHandle.Invalidate();
			Deactivate();
		},
		SecondDuration, false);

}

void APoolingProjectile_Area::ApplyDamageToSecondTargets()
{
	for (AActor* Target : SecondTargets)
	{
		if (Target)
		{
			// 모든 타겟에게 데미지 적용
			UGameplayStatics::ApplyDamage(Target, Damage, nullptr, nullptr, DamageType);
		}
	}
}

void APoolingProjectile_Area::OnSecondAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player") && !OtherActor->ActorHasTag("Bullet"))
	{
		SecondTargets.Add(OtherActor);
	}
}

void APoolingProjectile_Area::OnSecondAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SecondTargets.Remove(OtherActor);
}
