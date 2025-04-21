// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBaseActor.h"
#include "Components/SphereComponent.h"
#include "TowerDefence/Enemy/EnemyBase.h"
#include "TowerDefence/DEPRECATED_Tower/TowerBaseActor.h"

// Sets default values
AGunBaseActor::AGunBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);
	GunMesh->SetRelativeScale3D(FVector(ScaleFactor));
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(Root);
	MuzzleLocation->SetRelativeLocation(FVector::ForwardVector * 100.0f); // 총구 위치 설정(에디터에서 상세조정필요)

	SightSensor = CreateDefaultSubobject<USphereComponent>(TEXT("SightSensor"));
	SightSensor->SetupAttachment(Root);	
	SightSensor->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	GunDatas.Reserve(3); // 레벨 1~3까지의 총기 데이터 배열 크기 설정
}

// Called when the game starts or when spawned
void AGunBaseActor::BeginPlay()
{
	Super::BeginPlay();

	//check(GunDatas[0] != nullptr);		// 조건이 충족하지 않으면 종료
	//ensure(GunDatas[0] != nullptr);		// 조건이 충족하지 않으면 경고 메시지 출력 후 계속 진행
	//verify(GunDatas[0] != nullptr);		// 조건이 충족하지 않으면 종료(릴리즈에서는 생략)
}

// Called every frame
void AGunBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 총이 첫번째 타	겟을 바라보도록 회전
	LookFirstTarget(DeltaTime);
}

void AGunBaseActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SightSensor->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGunBaseActor::OnSightOverlapBegin); // 시야 센서의 겹침 시작 이벤트 바인딩
	SightSensor->OnComponentEndOverlap.AddUniqueDynamic(this, &AGunBaseActor::OnSightOverlapEnd); // 시야 센서의 겹침 종료 이벤트 바인딩
}

void AGunBaseActor::OnInitialize(ATowerBaseActor* InOwnerTower)
{
	OwnerTower = InOwnerTower;
	SetGunLevel(1); // 레벨 1의 총기 데이터로 초기화
}

void AGunBaseActor::SetGunLevel(int Level)
{
	if (Level > 0 && Level <= GunDatas.Num())
	{
		CurrentGunData = GunDatas[Level - 1];	// 레벨에 맞는 총기 데이터 선택
		verify(CurrentGunData != nullptr);		// 선택된 총기 데이터가 유효한지 확인

		float Modifier = OwnerTower->GetBuffModifierValue(ETowerBuffModifier::Range);			
		SightSensor->SetSphereRadius(CurrentGunData->Range * Modifier); // 시야 반경 설정

		if (!TargetEnemies.IsEmpty())	// 적이 있다 == Shoot타이머 실행중
		{
			ShootStop();	// 기존 타이머 정지
			ShootStart();	// 새로운 총기 데이터에 맞게 발사 시작
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid gun level: %d"), Level);
	}
}

void AGunBaseActor::RefreshBuffModifiers()
{
	// 총기 모디파이어 재적용
	if (CurrentGunData)
	{
		// 시야 반경 재설정
		float Modifier = OwnerTower->GetBuffModifierValue(ETowerBuffModifier::Range);
		SightSensor->SetSphereRadius(CurrentGunData->Range * Modifier); 

		// 발사 속도 재설정
		bIsShootingReset = true; // 발사 속도 재설정 플래그 설정
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGunData is nullptr!"));
	}
}

void AGunBaseActor::OnSightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor); // 적 캐릭터인지 확인
		if (Enemy)
		{
			if (TargetEnemies.IsEmpty())
			{
				ShootStart(FireFirstDelay); // 적 캐릭터가 처음 감지되면 발사 시작
			}

			TargetEnemies.AddUnique(Enemy); // 적 캐릭터를 목록에 추가
			PrintEnemyList();
		}
	}
}

void AGunBaseActor::OnSightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor); // 적 캐릭터인지 확인
		if (Enemy)
		{
			TargetEnemies.Remove(Enemy); // 적 캐릭터를 목록에서 제거
			PrintEnemyList();

			if (TargetEnemies.IsEmpty())
			{
				ShootStop(); // 적 캐릭터가 모두 사라지면 발사 중지
			}
		}
	}
}

void AGunBaseActor::PrintEnemyList()
{
	FString EnemyList;
	for (AEnemyBase* Enemy : TargetEnemies)
	{
		if (Enemy)
		{
			EnemyList += Enemy->GetName() + TEXT(", ");
		}
	}
	EnemyList += TEXT("Total: ") + FString::FromInt(TargetEnemies.Num());

	UE_LOG(LogTemp, Warning, TEXT("Enemies in range: [ %s ]"), *EnemyList);
}

void AGunBaseActor::ShootStart(float InFirstDelay)
{
	// 발사 시작
	float Modifier = OwnerTower->GetBuffModifierValue(ETowerBuffModifier::FireRate);
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		ShootTimerHandle, 
		this, &AGunBaseActor::Shoot, 
		1 / (CurrentGunData->FireRate * Modifier), true, InFirstDelay); // 발사 주기 설정
	//UE_LOG(LogTemp, Warning, TEXT("[ %s ] Shoot Start! : [%.2f] * [%.2f] "), 
	//	*this->GetActorLabel(), CurrentGunData->FireRate, Modifier);
}

void AGunBaseActor::ShootStop()
{
	// 발사 중지
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.IsTimerActive(ShootTimerHandle))
	{
		TimerManager.ClearTimer(ShootTimerHandle); // 발사 타이머 정지
	}
}

void AGunBaseActor::Shoot()
{
	UWorld* World = GetWorld();
	
	// 발사 로직 구현
	//FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));

	int Minutes = FMath::FloorToInt(World->TimeSeconds / 60);
	float Seconds = FMath::Fmod(World->TimeSeconds, 60.0f);
	FString TimeString = FString::Printf(TEXT("%02d:%05.3f"), Minutes, Seconds);
	UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Shooting!"), *TimeString, *this->GetActorLabel());

	if (bIsShootingReset)
	{
		bIsShootingReset = false; // 발사 속도 재설정 플래그 초기화
		ShootStop();	// 기존 타이머 정지
		ShootStart();	// 새로운 총기 데이터에 맞게 발사 시작
	}
}

void AGunBaseActor::LookFirstTarget(float DeltaTime)
{
	if (!TargetEnemies.IsEmpty())
	{
		int32 Count = FMath::Min(CurrentGunData->TargetCount, TargetEnemies.Num()); // 공격할 적의 수
		FVector TargetLocation = FVector::ZeroVector; // 타겟 위치 초기화
		for (int32 i = 0; i < Count; i++)
		{
			if (TargetEnemies[i])
			{
				TargetLocation += TargetEnemies[i]->GetActorLocation(); // 타겟 위치 누적
			}
		}
		TargetLocation /= Count; // 평균 위치 계산
		

		FVector Direction = TargetLocation - GetActorLocation(); // 총기 위치에서 적 캐릭터 평균 위치로의 방향 벡터
		Direction.Z = 0.0f;		// Yaw 회전만 고려하기 위해 높이(Z) 제거
		Direction.Normalize();	// 방향 벡터 정규화

		FRotator NewRotation = Direction.Rotation(); // 방향 벡터를 회전으로 변환

		NewRotation = FMath::RInterpConstantTo(GetActorRotation(), NewRotation, DeltaTime, 360.0f);
		SetActorRotation(NewRotation); // 총기 회전 설정
	}
}

float AGunBaseActor::GetModifierForDebuff(EDebuffType InType)
{
	float DebuffModifier = 1.0f;
	switch (CurrentGunData->ShotData->DebuffType)
	{
	case EDebuffType::DotDamage:
		DebuffModifier = OwnerTower->GetBuffModifierValue(ETowerBuffModifier::Damage);
		break;
	case EDebuffType::Slow:
	case EDebuffType::Stun:
	default:
		break;
	}

	return DebuffModifier;
}
