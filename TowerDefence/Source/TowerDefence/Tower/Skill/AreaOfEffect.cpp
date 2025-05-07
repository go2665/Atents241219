// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaOfEffect.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/Tower/Data/SkillDataAsset.h"
#include "TowerDefence/Tower/Data/AoeDataAsset.h"

// Sets default values
AAreaOfEffect::AAreaOfEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Sensor = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sensor"));
	Sensor->SetupAttachment(Root);
	Sensor->SetCollisionProfileName(TEXT("NoCollision"));
	Sensor->SetCapsuleHalfHeight(Radius * 2.0f);
	Sensor->SetCapsuleRadius(Radius);

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AAreaOfEffect::BeginPlay()
{
	Super::BeginPlay();
	Sensor->OnComponentBeginOverlap.AddDynamic(this, &AAreaOfEffect::OnTargetOverlapBegin);    
	Sensor->OnComponentEndOverlap.AddDynamic(this, &AAreaOfEffect::OnTargetOverlapEnd);
}

void AAreaOfEffect::OnInitialize(const USkillDataAsset& InSkillData, int32 InTowerLevel)
{
	if (bShowDebugInfo)
	{
		int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
		float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
		FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
		UE_LOG(LogTemp, Warning, TEXT("[%s] OnInitialize"), *TimeString);
	}

	int32 SkillLevel = FMath::Min(InTowerLevel, InSkillData.LevelData.Num() - 1);
	const FSkillLevelData& SkillLevelData = InSkillData.LevelData[SkillLevel];

	int32 AoELevel = FMath::Min(InTowerLevel, InSkillData.AoeData->LevelData.Num() - 1);
	const FAoeLevelData& AoeLevelData = InSkillData.AoeData->LevelData[AoELevel];

	DamageType = InSkillData.AoeData->DamageType;	// 데미지 타입 설정
	VFX->SetAsset(InSkillData.SkillEffect);			// 파티클 에셋 설정
	VFX->SetWorldLocation(GetActorLocation());		// 파티클 위치 설정
	VFX->Activate();								// 파티클 활성화

	Radius = SkillLevelData.Radius;					// 반지름 설정
	Sensor->SetCapsuleHalfHeight(Radius * 2.0f);
	Sensor->SetCapsuleRadius(Radius);

	SetLifeSpan(AoeLevelData.Duration);				// 지속시간 설정	
	Damage = AoeLevelData.Damage;					// 즉시 데미지
	DotDamage = AoeLevelData.DotDamage;				// 틱당 데미지
	DotInterval = AoeLevelData.DotInterval;			// 틱당 데미지 주는 시간 간격

	Sensor->SetCollisionProfileName(TEXT("OverlapEnemyOnly"));	// 센서 켜기

	// 즉석 데미지 적용하기

}

void AAreaOfEffect::OnTargetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 로그로 시간과 들어온 액터의 이름을 출력	
	if (bShowDebugInfo)
	{
		int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
		float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
		FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
		UE_LOG(LogTemp, Warning, TEXT("[%s] Overlap Begin: %s"), *TimeString , *OtherActor->GetActorNameOrLabel());
	}	

	if (AffectedActors.IsEmpty())
	{
		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();

		TimerManager.SetTimer(
			TimerHandle,
			this,
			&AAreaOfEffect::OnTickDamage,
			DotInterval,
			true);
	}
	AffectedActors.AddUnique(OtherActor); // 겹친 액터를 추가
}

void AAreaOfEffect::OnTargetOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bShowDebugInfo)
	{
		int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
		float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
		FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
		UE_LOG(LogTemp, Warning, TEXT("[%s] Overlap End: %s"), *TimeString, *OtherActor->GetActorNameOrLabel());
	}

	AffectedActors.RemoveSingle(OtherActor); // 겹친 액터를 제거
	if (AffectedActors.IsEmpty())
	{
		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(TimerHandle);
	}
}

void AAreaOfEffect::OnTickDamage()
{
	for(AActor* Target : AffectedActors)
	{
		UGameplayStatics::ApplyDamage(
			Target,
			DotDamage,
			nullptr,
			this,
			DamageType); // 데미지 적용
	}
}

