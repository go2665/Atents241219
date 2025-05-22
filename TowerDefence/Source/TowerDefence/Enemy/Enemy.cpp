// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine/DamageEvents.h"
#include "TowerDefence/Defines/DamageAttribute/FireDamageType.h"
#include "TowerDefence/Defines/DamageAttribute/IceDamageType.h"
#include "TowerDefence/Defines/DamageAttribute/LightningDamageType.h"
#include "TowerDefence/Defines/DamageAttribute/PoisonDamageType.h"
#include "TowerDefence/Tower/Projectile.h"
#include "Components/SplineComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 설정
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 메쉬 컴포넌트 생성 및 설정
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(Root);
	EnemyMesh->SetCollisionProfileName(TEXT("Enemy")); // 적 캐릭터의 충돌 프로파일 설정

	// 태그 설정
	Tags.Add(FName("Enemy")); // 태그 추가
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentDistance += DeltaTime * Speed; // 스플라인을 따라 이동한 거리 계산

	if (SpawnerSpline)
	{
		// 스플라인을 따라 이동
		FVector NewLocation = SpawnerSpline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
		FRotator NewRotation = SpawnerSpline->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(NewLocation + Offset, NewRotation); // 위치와 회전 설정

		// 스플라인의 끝에 도달했는지 확인
		if (CurrentDistance > SpawnerSpline->GetSplineLength())
		{
			// 스플라인의 끝에 도달했을 때의 처리
			if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] Reached the end of the spline!"), *this->GetActorLabel());
			
			Die(true);	// 골인 지점에 도착한 채로 죽음 = 공격
		}
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 데미지 처리
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ActualDamage *= (1 + GetModifier(EEffectModifier::TakeDamage));	// 데미지 받는 배율 적용
	if (DamageEvent.DamageTypeClass == GetEnemyData()->WeakType)
	{
		ActualDamage *= 2.0f;			// 약점 속성에 대한 데미지 배가
	}

	if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] Take Damage: %.1f"), *this->GetActorLabel(), ActualDamage);
	SetHealth(CurrentHealth - ActualDamage); // 체력 설정

	FVector Location = FVector::ZeroVector;

	// 발사체에게 맞은 대상을 알림
	if (DamageCauser)
	{
		// 적 캐릭터가 맞은 발사체에 이팩트가 있다면 이팩트 추가
		AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
		if (Projectile)
		{
			Projectile->AddHitEnemy(this); // 발사체에 적 기록
		}

		// 피격 VFX 생성 위치 결정(발사체 위치에서 재생)
		if (HitEffectAsset)
		{
			Location = DamageCauser->GetActorLocation() + HitEffectOffset;
		}		
	}
	else
	{
		// 피격 VFX 생성 위치 결정(적 캐릭터 위치에서 재생)
		if (HitEffectAsset)
		{
			FVector RandomOffset = FMath::VRand();
			RandomOffset.Z = 0.0f;
			RandomOffset *= 50.0f;
			Location = GetActorLocation() + HitEffectOffset + RandomOffset;
		}
	}

	// 피격 VFX 생성
	if (HitEffectAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffectAsset,
			Location,
			FRotator::ZeroRotator,
			FVector(1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease
		);
	}

	return ActualDamage;
}

void AEnemy::OnSpawn(USplineComponent* InSpline, const FVector& InOffset)
{
	// 스플라인 저장
	SpawnerSpline = InSpline; 

	// 스플라인의 시작 위치로 설정
	SetActorLocation(SpawnerSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));

	// 스플라인의 시작 회전으로 설정
	SetActorRotation(SpawnerSpline->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World));

	CurrentDistance = 0.0f;			
	Offset = InOffset;
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Offset: %s"), *this->GetActorLabel(), *Offset.ToString());
}

bool AEnemy::AddEffect(EEffectType InType, int32 InLevel)
{
	if (EffectComponent)
	{
		// EffectComponent에 이팩트 추가
		return EffectComponent->AddEffect(InType, InLevel);
	}
	return false;
}

bool AEnemy::RemoveEffect(EEffectType InType)
{
	if (EffectComponent)
	{
		// EffectComponent에 이팩트 제거
		return EffectComponent->RemoveEffect(InType);
	}
	return false;
}

void AEnemy::ApplyModifiers(const TMap<EEffectModifier, float>* InModifierMap)
{
	EffectModifiers = InModifierMap;	// 모디파이어 맵의 주소를 저장

	// Speed 등에 기본값과 모디파이어를 곱한 값(최종값)을 설정(없으면 기본값 사용)

	// 모디파이어 있으면 모디파이어 값을 곱할것
	Speed = FMath::Max(0.0f, GetEnemyData()->Speed * (1 + GetModifier(EEffectModifier::MoveSpeed))); // 이동 속도
	Defence = FMath::Max(0.0f, GetEnemyData()->Defence * (1 + GetModifier(EEffectModifier::Defence))); // 방어력

	if (bShowDebugInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Speed: (%.1f, %.1f), Defence: (%.1f, %.1f)"), 
			*this->GetActorLabel(), 
			GetEnemyData()->Speed, GetModifier(EEffectModifier::MoveSpeed),
			GetEnemyData()->Defence, GetModifier(EEffectModifier::Defence));
	}
}

void AEnemy::Die(bool bGoalArrived)
{
	if (bGoalArrived)
	{
		OnEnemyAttack.Broadcast(GetEnemyData()->Damage); // 골인 지점에 도달했을 때 호출되는 델리게이트 호출
	}
	else
	{
		OnEnemyKilled.Broadcast(GetEnemyData()->Gold);	// 적 캐릭터가 골에 도착전에 죽었을 때의 처리
	}
	bIsAlive = false; // 죽었다고 표시
	//Destroy(); // 적 캐릭터 삭제(발사체가 타겟으로 하고 있을 때는 메모리 상에서 실제 삭제는 안됨. 게임에서는 삭제 됨)

	GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReleaseObject(this); // 적 캐릭터 풀로 반환
}

void AEnemy::SetHealth(float InHealth)
{
	CurrentHealth = InHealth;
	if (CurrentHealth <= 0.0f)
	{
		Die(false);
		if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] is dead!"), *this->GetActorLabel());
	}
	else
	{
		// 적 캐릭터가 살아있을 때의 처리
		if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] Current Health: %.1f"), *this->GetActorLabel(), CurrentHealth);
	}
}

void AEnemy::OnInitialize()
{
	// UEffectComponent를 찾아서 EffectComponent에 저장
	EffectComponent = FindComponentByClass<UEffectComponent>();

	ensure(GetEnemyData() != nullptr); // EnemyData가 nullptr이 아닌지 확인

	if (GetEnemyData())
	{
		CurrentHealth = GetEnemyData()->Health;	// 적 캐릭터의 체력 설정
		Speed = GetEnemyData()->Speed;			// 적 캐릭터의 이동 속도 설정
		Defence = GetEnemyData()->Defence;		// 적의 방어력 설정
		// 속성과 골드는 데이터에서 직접 확인
	}

	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
	OnEnemyAttack.AddUObject(GameMode, &ATowerDefenceGameMode::SubtractHealth);
	OnEnemyKilled.AddUObject(GameMode, &ATowerDefenceGameMode::OnEnemyKilled);
}

void AEnemy::OnActivate()
{
}

void AEnemy::OnDeactivate()
{
	CurrentDistance = 0.0f;
	CurrentHealth = GetEnemyData()->Health;
}
