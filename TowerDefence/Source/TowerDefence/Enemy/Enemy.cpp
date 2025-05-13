// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine/DamageEvents.h"
#include "TowerDefence/Defines/DamageAttribute/FireDamageType.h"
#include "TowerDefence/Defines/DamageAttribute/IceDamageType.h"
#include "TowerDefence/Defines/DamageAttribute/LightningDamageType.h"
#include "TowerDefence/Defines/DamageAttribute/PoisonDamageType.h"
#include "TowerDefence/Tower/Projectile.h"
#include "Components/SplineComponent.h"

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

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

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
		SetActorLocationAndRotation(NewLocation, NewRotation); // 위치와 회전 설정

		// 스플라인의 끝에 도달했는지 확인
		if (CurrentDistance > SpawnerSpline->GetSplineLength())
		{
			// 스플라인의 끝에 도달했을 때의 처리
			if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] Reached the end of the spline!"), *this->GetActorLabel());
			
			OnEnemyAttack.Broadcast(GetEnemyData()->Damage); // 골인 지점에 도달했을 때 호출되는 델리게이트 호출			
			Destroy(); // 적 캐릭터 삭제
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

	// 발사체에게 맞은 대상을 알림
	if (DamageCauser)
	{
		// 적 캐릭터가 맞은 발사체에 이팩트가 있다면 이팩트 추가
		AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
		if (Projectile)
		{
			Projectile->AddHitEnemy(this); // 발사체에 적 기록
		}
	}

	return ActualDamage;
}

void AEnemy::InitializeEnemy(USplineComponent* InSpline)
{
	SpawnerSpline = InSpline; // 스플라인 저장
	CurrentDistance = 0.0f;
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

void AEnemy::SetHealth(float InHealth)
{
	CurrentHealth = InHealth;
	if (CurrentHealth <= 0.0f)
	{
		// 적 캐릭터가 죽었을 때의 처리
		OnEnemyKilled.Broadcast(GetEnemyData()->Gold);

		//Destroy(); // 적 캐릭터 삭제

		if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] is dead!"), *this->GetActorLabel());
	}
	else
	{
		// 적 캐릭터가 살아있을 때의 처리
		if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] Current Health: %.1f"), *this->GetActorLabel(), CurrentHealth);
	}
}
