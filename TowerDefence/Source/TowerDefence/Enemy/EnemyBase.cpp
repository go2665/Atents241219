// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Engine/DamageEvents.h"
//#include "TowerDefence/Shot/Attribute/TowerDamageType.h"
#include "TowerDefence/Shot/Attribute/FireDamageType.h"
#include "TowerDefence/Shot/Attribute/IceDamageType.h"
#include "TowerDefence/Shot/Attribute/LightningDamageType.h"
#include "TowerDefence/Shot/Attribute/PoisonDamageType.h"
#include "TowerDefence/Shot/Debuff/DebuffComponent.h"
#include "TowerDefence/Shot/ShotProjectileBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 설정
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 메쉬 컴포넌트 생성 및 설정
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(Root);
	EnemyMesh->SetCollisionProfileName(TEXT("Enemy")); // 적 캐릭터의 충돌 프로파일 설정

	// 디버프 컴포넌트 생성 및 설정
	DebuffComponent = CreateDefaultSubobject<UDebuffComponent>(TEXT("DebuffComponent"));

	// 태그 설정
	Tags.Add(FName("Enemy")); // 태그 추가
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DeltaTime * Speed * (1 - SpeedModifier) * GetActorForwardVector();

}


float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		
	ActualDamage *= DamageModifier;	// 데미지 배율 적용
	if (DamageEvent.DamageTypeClass == WeakType)
	{
		ActualDamage *= 2.0f;			// 약점 속성에 대한 데미지 배가
	}

	SetCurrentHealth(CurrentHealth - ActualDamage); // 체력 설정

	if (DamageCauser)
	{
		// 적 캐릭터가 맞은 발사체에 디버프가 있다면 디버프 추가
		AShotProjectileBase* Shot = Cast<AShotProjectileBase>(DamageCauser);
		if (Shot)
		{
			Shot->AddHitEnemy(this); // 발사체에 적 추가
		}
	}
	return ActualDamage;
}

void AEnemyBase::UpdateSpeedModifier(EDebuffType IgnoreType)
{
	SpeedModifier = DebuffComponent->GetMaxiumModifierValue(IgnoreType, EDebuffModifier::Speed);
}

void AEnemyBase::UpdateDamageModifier(EDebuffType IgnoreType)
{
	DamageModifier = DebuffComponent->GetMaxiumModifierValue(IgnoreType, EDebuffModifier::Damage);
}

void AEnemyBase::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
	if (CurrentHealth <= 0.0f)
	{
		// 적 캐릭터가 죽었을 때의 처리
		//Destroy(); // 적 캐릭터 삭제

		UE_LOG(LogTemp, Warning, TEXT("[%s] is dead!"), *this->GetActorLabel());
	}
	else
	{
		// 적 캐릭터가 살아있을 때의 처리
		UE_LOG(LogTemp, Warning, TEXT("[%s] Current Health: %.1f"), *this->GetActorLabel(), CurrentHealth);
	}
}

