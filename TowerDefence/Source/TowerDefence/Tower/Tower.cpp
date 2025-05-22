// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Tower/Cannon.h"
#include "TowerDefence/Tower/Projectile.h"
#include "TowerDefence/Tower/Data/CannonDataAsset.h"
#include "TowerDefence/Tower/Data/ShotDataAsset.h"
#include "TowerDefence/Tower/UI/TowerUpgradeWidget.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Enemy/Enemy.h"
#include "TowerDefence/Components/Effect/EffectComponent.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"

// Sets default values
ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TowerBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBodyMesh"));
	TowerBodyMesh->SetupAttachment(Root);
	TowerBodyMesh->SetCollisionProfileName(TEXT("Tower"));

	UpgradeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpgradeWidget"));
	UpgradeWidget->SetupAttachment(Root);
	UpgradeWidget->SetWidgetSpace(EWidgetSpace::Screen);	
	UpgradeWidget->SetDrawSize(FVector2D(200.0f, 600.0f));	// 위젯 크기 설정
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	verify(CannonData != nullptr);
	verify(ShotData != nullptr);

	TowerLevel = 0;
	TowerLevelCap = CannonData->LevelData.Num();

    // UEffectComponent를 찾아서 EffectComponent에 저장
    EffectComponent = FindComponentByClass<UEffectComponent>();

	// 타워 데이터 초기화
	UpdateData();
		
	// 대포 인스턴스가 없으면 생성하고 초기화
	if (!CannonInstance)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CannonInstance = GetWorld()->SpawnActor<ACannon>(
			CannonData->CannonClass,
			FTransform::Identity,
			SpawnParams);
		CannonInstance->AttachToComponent(
			TowerBodyMesh,
			FAttachmentTransformRules::KeepRelativeTransform, TEXT("TowerRoof"));

		CannonInstance->OnInitialize(this, CannonData->FireOrder);			// 발사 순서 기준
		CannonInstance->OnCannonFire.AddDynamic(this, &ATower::TowerFire);	// 발사 시 타워에 신호 전달
	}
	
	// 타워 업그레이드 UI 위젯 초기화
	UpgradeWidgetInstance = Cast<UTowerUpgradeWidget>(UpgradeWidget->GetUserWidgetObject());
	if (UpgradeWidgetInstance)
	{
		UpgradeWidgetInstance->OnInitialize();
		UpgradeWidgetInstance->OnUpgradeClicked.BindUFunction(this, FName("TowerLevelUp"));
		UpgradeWidgetInstance->OnSellClicked.BindUFunction(this, FName("TowerSell"));
	}

	// 타워 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 열기)
	OnClicked.AddDynamic(this, &ATower::OnTowerClicked);

	// 타워 밖 클릭시 실행 함수 연결(타워 업그레이드 UI 위젯 닫기)
	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnMouseClickInput.AddUObject(this, &ATower::OnScreenClicked);
	}
}

void ATower::TowerLevelUp()
{
	if (TowerLevel < TowerLevelCap)
	{
		float UpgradeCost = GetCannonLevelData().UpgradeCost;
		ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode->UseGold(UpgradeCost))	// 업그레이드 비용 차감 시도
		{
			TowerLevel++;					// 타워 레벨 증가
			SellCost += UpgradeCost * 0.5f; // 판매 비용 증가
			
			UpdateData();	// 타워 데이터 갱신

			// 레벨업 델리게이트 방송. TowerLevel 전달(캐논, 버퍼에 방송)
			OnTowerLevelUp.Broadcast(TowerLevel);
		}
	}
}

void ATower::TowerSell()
{
	if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Sell!"), *this->GetActorNameOrLabel());
	
	OnTowerSell.Broadcast(SellCost);	// 타워가 팔렸다고 알리기

	if (CannonInstance)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Sell : Cannon Destroyed!"), *CannonInstance->GetActorNameOrLabel());
		CannonInstance->Destroy();	// 대포 인스턴스 삭제
		CannonInstance = nullptr;
	}

	Destroy();	// 타워 삭제	
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Sell : Tower Destroyed!"), *this->GetActorNameOrLabel());
}

void ATower::TowerFire(const TArray<AEnemy*>& InTargetEnemies)
{
	//if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Tower Fire!"), *this->GetActorNameOrLabel());
	//Test_PrintFireTargetList(InTargetEnemies);	// 공격하는 적 목록 출력

	if (ShotData->IsProjectile())
	{
		// 발사체 생성해서 발사
		ShootProjectile(InTargetEnemies);
	}
	else
	{
		// 히트스캔으로 공격
		ShootHitScan(InTargetEnemies);
	}
}

bool ATower::AddEffect(EEffectType InType, int32 InLevel)
{
	if (EffectComponent)
	{
		// EffectComponent에 이팩트 추가
		return EffectComponent->AddEffect(InType, InLevel);
	}
	return false;
}

bool ATower::RemoveEffect(EEffectType InType)
{
	if (EffectComponent)
	{
		// EffectComponent에 이팩트 제거
		return EffectComponent->RemoveEffect(InType);
	}
	return false;
}

void ATower::ApplyModifiers(const TMap<EEffectModifier, float>* InModifierMap)
{
	EffectModifiers = InModifierMap;	// 모디파이어 맵의 주소를 저장
	UpdateData();
}

void ATower::OnTowerClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	//UE_LOG(LogTemp, Warning, TEXT("ATower:OnTowerClicked : Mouse Clicked!"));

	// 타워 클릭 시 업그레이드 UI 위젯 열기
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		if (UpgradeWidgetInstance && bIsActivated)
		{
			UpgradeWidgetInstance->OpenUpgradeWidget(GetCannonLevelData().UpgradeCost);
			//if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Clicked Tower!"), *this->GetActorNameOrLabel());
		}
	}
}

void ATower::OnScreenClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("ATower:OnScreenClicked : Mouse Clicked!"));
	if (UpgradeWidgetInstance)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		AActor* HitActor = nullptr;	// 클릭한 액터를 저장할 변수
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult))	// ECC_GameTraceChannel2(Tower)로 트레이스
		{
			HitActor = HitResult.GetActor();
		}
		//if (HitActor)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Clicked on: %s"), *HitActor->GetActorNameOrLabel());
		//}

		if (HitActor != this)
		{
			// 타워가 아닌 다른 액터 클릭 시 업그레이드 UI 위젯 닫기
			UpgradeWidgetInstance->CloseUpgradeWidget();
			//if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Upgrade Widget!"), *this->GetActorNameOrLabel());
		}
	}
	//else
	//{
	//	if (bShowDebugInfo) UE_LOG(LogTemp, Warning, TEXT("[%s] : Not Close Upgrade Widget!"), *InClickedTower->GetActorNameOrLabel());
	//}
}

void ATower::UpdateData()
{
	// Damage, Range, FireRate, TargetCount 등에 기본값과 모디파이어를 곱한 값을 설정(없으면 기본값 사용)
	
	// 모디파이어 있으면 모디파이어 값을 곱할것
	Damage = FMath::Max(1.0f, GetShotLevelData().Damage * (1 + GetModifier(EEffectModifier::Attack))); // 1.0은 음수나 0이 되는 것을 방지하기 위한 것
	Range = FMath::Max(1.0f, GetCannonLevelData().Range * (1 + GetModifier(EEffectModifier::FireRange)));
	FireRate = FMath::Max(0.1f, GetCannonLevelData().FireRate * (1 + GetModifier(EEffectModifier::FireRate)));

	// 현재 모디파이어 없음. 생기면 추가
	TargetCount = GetCannonLevelData().TargetCount; 
}

void ATower::ShootProjectile(const TArray<AEnemy*>& InTargetEnemies)
{
	UWorld* World = GetWorld();

	// ProjectileClass를 MuzzleLocation에 스폰
	int32 Count = FMath::Min(GetCannonLevelData().TargetCount, InTargetEnemies.Num()); // 공격할 적의 수
	for (int32 i = 0; i < Count; i++)
	{
		AProjectile* Projectile = Cast<AProjectile>(
			World->GetSubsystem<UObjectPoolSubsystem>()->GetObject(ShotData->ProjectileType));

		if (Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] : Projectile is Spawn!"), *Projectile->GetActorNameOrLabel());
			Projectile->OnSpawn(
				CannonInstance->GetMuzzleTransform(),
				ShotData,
				InTargetEnemies[i],
				TowerLevel,
				Damage,
				bShowDebugInfo
			);

			if (bShowDebugInfo)
			{
				// 발사한 타워의 이름과 발사 시간을 로그로 출력(소수점 둘째자리 까지 초단위로 출력)
				int Minutes = FMath::FloorToInt(World->TimeSeconds / 60);
				float Seconds = FMath::Fmod(World->TimeSeconds, 60.0f);
				FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
				UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] : Shoot!"), *TimeString, *this->GetActorNameOrLabel());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] : Projectile is nullptr!"), *this->GetActorNameOrLabel());
		}
	}
}

void ATower::ShootHitScan(const TArray<AEnemy*>& InTargetEnemies)
{
	int32 Count = FMath::Min(GetCannonLevelData().TargetCount, InTargetEnemies.Num()); // 공격할 적의 수
	for (int32 i = 0; i < Count; i++)
	{
		TArray<AEnemy*> HitEnemies; // 맞은 적 캐릭터 배열
		bool bHit = LineTraceToTarget(InTargetEnemies[i], HitEnemies);
		if (bHit)
		{
			for (auto HitEnemy : HitEnemies) // HitEnemies는 Shot데이터가 (bIsSplashAttack == true)일때 여러명일 수 있다.
			{
				// 맞은 적 캐릭터에 대한 처리

				// 데미지 처리
				UGameplayStatics::ApplyDamage(
					HitEnemy,		// 적 캐릭터
					Damage,			// 데미지
					nullptr,		// 행위자
					nullptr,		// 공격자
					ShotData->DamageType // 총알의 속성 타입
				);

				// 디버프 처리
				int ShotLevel = ShotData->GetSafeLevel(TowerLevel); // 안전한 레벨 구하기
				HitEnemy->AddEffect(ShotData->GetLevelData(ShotLevel).EffectType, ShotLevel);
			}
		}
	}
}

bool ATower::LineTraceToTarget(AActor* InTarget, TArray<AEnemy*>& OutHitTargets)
{
	OutHitTargets.Empty(); // Out파라메터는 초기화하고 사용하기
	bool bHit = false;

	FVector Direction = InTarget->GetActorLocation() - CannonInstance->GetMuzzleLocation(); // 총구에서 적 캐릭터까지의 방향 벡터
	Direction.Z = 0.0f;			// Z축 방향 제거(2D 평면에서의 거리 계산을 위해)
	Direction.Normalize();		// 방향 벡터 정규화

	FVector Start = CannonInstance->GetMuzzleLocation();	// 총구 위치
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Start = %s"), *this->GetActorNameOrLabel(), *Start.ToString());
	FVector End = Start + Range * Direction; // 총구에서 적 캐릭터까지의 거리만큼 나간 위치

	UWorld* World = GetWorld();

	FCollisionQueryParams CollisionParams; // 충돌 쿼리 파라미터
	CollisionParams.AddIgnoredActor(this); // 쿼리에서 무시할 액터 추가

	if (ShotData->bIsSplashAttack)
	{
		TArray<FHitResult> HitResults;
		bHit = World->LineTraceMultiByObjectType(
			HitResults,			// 충돌 결과
			Start,				// 시작 위치
			End,				// 끝 위치
			FCollisionObjectQueryParams(ECC_GameTraceChannel1), // 충돌 오브젝트 채널
			CollisionParams
		);

		if (bHit)	// 맞았으면 맞은 시간과 대상 출력(단순 확인용)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor()); // 충돌한 액터가 적 캐릭터인지 확인
				if (HitEnemy)
				{
					//FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));
					//UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Hit!"), *TimeString, *HitEnemy->GetActorLabel());
					OutHitTargets.Add(HitEnemy); // 맞은 적 캐릭터를 배열에 추가

					if (bShowDebugInfo)
					{
						// 맞은 위치에 구 그리기
						DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Red, false, 1.0f);
					}
				}
			}
		}
	}
	else
	{
		FHitResult HitResult; // 충돌 결과를 저장할 변수
		bHit = World->LineTraceSingleByObjectType(
			HitResult,			// 충돌 결과
			Start,				// 시작 위치
			End,				// 끝 위치
			FCollisionObjectQueryParams(ECC_GameTraceChannel1), // 충돌 오브젝트 채널
			CollisionParams
		);

		if (bHit)	// 맞았으면 맞은 시간과 대상 출력(단순 확인용)
		{
			AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor()); // 충돌한 액터가 적 캐릭터인지 확인
			if (HitEnemy)
			{
				//FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));
				//UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Hit!"), *TimeString, *HitEnemy->GetActorLabel());
				OutHitTargets.Add(HitEnemy); // 맞은 적 캐릭터를 배열에 추가

				if (bShowDebugInfo)
				{
					// 맞은 위치에 구 그리기
					DrawDebugSphere(World, HitResult.Location, 10.0f, 12, FColor::Red, false, 1.0f); 
				}
			}
		}
	}

#if WITH_EDITOR
	if (bShowDebugInfo)
	{
		FColor LineColor = bHit ? FColor::Red : FColor::Green; // 충돌 여부에 따라 선 색상 설정
		DrawDebugLine(World, Start, End, LineColor, false, 1.0f, 0, 1.0f); // 선 그리기
	}
#endif

	return bHit;
}

void ATower::Test_PrintFireTargetList(const TArray<AEnemy*>& InTargetEnemies)
{
	FString EnemyList;
	for (AEnemy* Enemy : InTargetEnemies)
	{
		if (Enemy)
		{
			EnemyList += Enemy->GetName() + TEXT(", ");
		}
	}
	EnemyList += TEXT("Total: ") + FString::FromInt(InTargetEnemies.Num());

	UE_LOG(LogTemp, Warning, TEXT("Enemies in range: [ %s ]"), *EnemyList);
}