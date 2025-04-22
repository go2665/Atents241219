// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SphereComponent.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Enemy/EnemyBase.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(Root);
	CannonMesh->SetRelativeScale3D(FVector(ScaleFactor));				// 메시 크기 조절
	CannonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 비활성화

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(Root);
	MuzzleLocation->SetRelativeLocation(FVector::ForwardVector * 100.0f); // 총구 위치 설정(에디터에서 상세조정필요)

	SightSensor = CreateDefaultSubobject<USphereComponent>(TEXT("SightSensor"));
	SightSensor->SetupAttachment(Root);
	SightSensor->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	// 오버랩만 되도록 설정
}

void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 총이 첫번째 타	겟을 바라보도록 회전
	LookFirstTarget(DeltaTime);
}

void ACannon::OnInitialize(ATower* InTower, EFireOrder InOrder)
{
	ParentTower = InTower;		// 부모 타워 설정
	FireOrder = InOrder;		// 발사 순서 설정
	OnModifierChange();			// 모디파이어 초기화

	// 시야 센서의 겹침 시작 이벤트 바인딩
	SightSensor->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACannon::OnSightOverlapBegin); 
	// 시야 센서의 겹침 종료 이벤트 바인딩
	SightSensor->OnComponentEndOverlap.AddUniqueDynamic(this, &ACannon::OnSightOverlapEnd); 
}

void ACannon::OnModifierChange()
{
	// 시야 반경 설정
	SightSensor->SetSphereRadius(ParentTower->GetRange()); 

	// 발사 속도 재적용
	if (!TargetEnemies.IsEmpty())		// 적이 있다 == Shoot타이머 실행중
	{
		bShootingResetFlag = true;		// 발사 속도 재적용 플래그 설정	
	}
}

void ACannon::OnSightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor); // 적 캐릭터인지 확인
		if (Enemy)
		{
			if (TargetEnemies.IsEmpty())
			{
				FireStart(FireFirstDelay); // 적 캐릭터가 처음 감지되면 발사 시작
			}

			TargetEnemies.AddUnique(Enemy); // 적 캐릭터를 목록에 추가
			//Test_PrintEnemyList();
		}
	}
}

void ACannon::OnSightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor); // 적 캐릭터인지 확인
		if (Enemy)
		{
			TargetEnemies.Remove(Enemy); // 적 캐릭터를 목록에서 제거
			//Test_PrintEnemyList();

			if (TargetEnemies.IsEmpty())
			{
				FireStop(); // 적 캐릭터가 모두 사라지면 발사 중지
			}
		}
	}
}

void ACannon::FireStart(float InFirstDelay)
{
	// 발사 시작
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		ShootTimerHandle,
		this, &ACannon::Fire,
		1 / ParentTower->GetFireRate(), true, InFirstDelay); // 발사 주기 설정
	//UE_LOG(LogTemp, Warning, TEXT("[ %s ] Shoot Start! : [%.2f] * [%.2f] "), 
	//	*this->GetActorLabel(), CurrentGunData->FireRate, Modifier);
}

void ACannon::Fire()
{
	// 공격 대상을 결정하는 방식이 거리로 되어 있으면 정렬 필요
	if (FireOrder == EFireOrder::Distance)
	{
		// 적을 거리의 오름차순으로 정렬하기
		FVector GunLocation = GetActorLocation();
		TargetEnemies.Sort([GunLocation](const AEnemyBase& A, const AEnemyBase& B)
			{
				float DistanceSquaredA = FVector::DistSquared(GunLocation, A.GetActorLocation());
				float DistanceSquaredB = FVector::DistSquared(GunLocation, B.GetActorLocation());
				return DistanceSquaredA < DistanceSquaredB;	// 오름차순 정렬
			});
	}

	// 공격 대상 결정
	int32 Count = FMath::Min(ParentTower->GetTargetCount(), TargetEnemies.Num()); // 공격할 적의 수
	TArray<AEnemyBase*> CurrentTargetEnemies;
	CurrentTargetEnemies.Reserve(Count);
	for (int32 i = 0; i < Count; i++)
	{
		if (TargetEnemies[i])
		{
			CurrentTargetEnemies.Add(TargetEnemies[i]); // 타겟 적 추가
		}
	}
	
	// 발사 델리게이트 호출
	OnCannonFire.Broadcast(CurrentTargetEnemies);	// 델리게이트 호출(ParentTower에서 처리)

	//UWorld* World = GetWorld();
	////FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));
	//int Minutes = FMath::FloorToInt(World->TimeSeconds / 60);
	//float Seconds = FMath::Fmod(World->TimeSeconds, 60.0f);
	//FString TimeString = FString::Printf(TEXT("%02d:%05.3f"), Minutes, Seconds);
	//UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Shooting!"), *TimeString, *this->GetActorLabel());

	if (bShootingResetFlag)
	{
		bShootingResetFlag = false; // 발사 속도 재설정 플래그 초기화
		FireStop();		// 기존 타이머 정지
		FireStart();	// 새로운 총기 데이터에 맞게 발사 시작
	}
}

void ACannon::FireStop()
{
	// 발사 중지
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.IsTimerActive(ShootTimerHandle))
	{
		TimerManager.ClearTimer(ShootTimerHandle); // 발사 타이머 정지
	}
}

void ACannon::LookFirstTarget(float DeltaTime)
{
	if (!TargetEnemies.IsEmpty())
	{
		// 공격할 적의 수(공격 가능한 수와 현재 타겟 적의 수 중 작은 값)
		int32 Count = FMath::Min(ParentTower->GetTargetCount(), TargetEnemies.Num()); 
		
		// 적이 여러명일 때를 위해 평균 위치 계산
		FVector TargetLocation = FVector::ZeroVector; // 타겟 위치 초기화
		for (int32 i = 0; i < Count; i++)
		{
			if (TargetEnemies[i])
			{
				TargetLocation += TargetEnemies[i]->GetActorLocation(); // 타겟 위치 누적
			}
		}
		TargetLocation /= Count; // 평균 위치 계산

		// 방향 결정
		FVector Direction = TargetLocation - GetActorLocation(); // 총기 위치에서 적 캐릭터 평균 위치로의 방향 벡터
		Direction.Z = 0.0f;		// Yaw 회전만 고려하기 위해 높이(Z) 제거
		Direction.Normalize();	// 방향 벡터 정규화

		// 해당 방향으로 회전
		FRotator NewRotation = Direction.Rotation(); // 방향 벡터를 회전으로 변환
		NewRotation = FMath::RInterpConstantTo(GetActorRotation(), NewRotation, DeltaTime, 360.0f);
		SetActorRotation(NewRotation); // 총기 회전 설정
	}
}

void ACannon::Test_PrintEnemyList()
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

