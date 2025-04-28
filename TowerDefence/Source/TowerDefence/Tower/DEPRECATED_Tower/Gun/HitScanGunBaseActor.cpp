// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanGunBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Enemy/DEPRECATED_Enemy/EnemyBase.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Effect/Debuff/DebuffComponent.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/TowerBaseActor.h"

void AHitScanGunBaseActor::Shoot()
{
	// Line : 선. 시작점과 끝점으로 정의(위치 2개)
	// Ray : 선. 시작점과 방향으로 정의(위치 + 벡터)

	//FVector Start = MuzzleLocation->GetComponentLocation();	// 총구 위치
	//FVector End = Start + MuzzleLocation->GetForwardVector() * CurrentGunData->Range; // 총구에서 앞쪽으로 사정거리만큼 나간 위치

	HitProcess();

	Super::Shoot();	// 리셋 처리만 있기 때문에 마지막에 호출
}

bool AHitScanGunBaseActor::LineTraceToTarget(FVector InTarget, TArray<AEnemyBase*>& OutHitTargets)
{
	OutHitTargets.Empty(); // Out파라메터는 초기화하고 사용하기

	bool bHit = false;
	FVector Start = GetActorLocation() + MuzzleLocation->GetRelativeLocation();	// 총구 위치
	FVector End = InTarget;									// 적 위치

	UWorld* World = GetWorld();
		
	FCollisionQueryParams CollisionParams; // 충돌 쿼리 파라미터
	CollisionParams.AddIgnoredActor(this); // 쿼리에서 무시할 액터 추가

	if (CurrentGunData->ShotData->bIsAreaAttack)
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
				AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitResult.GetActor()); // 충돌한 액터가 적 캐릭터인지 확인
				if (HitEnemy)
				{
					FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));
					UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Hit!"), *TimeString, *HitEnemy->GetActorLabel());
					OutHitTargets.Add(HitEnemy); // 맞은 적 캐릭터를 배열에 추가
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
			AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitResult.GetActor()); // 충돌한 액터가 적 캐릭터인지 확인
			if (HitEnemy)
			{
				FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));
				UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Hit!"), *TimeString, *HitEnemy->GetActorLabel());
				OutHitTargets.Add(HitEnemy); // 맞은 적 캐릭터를 배열에 추가
			}
		}
	}		

	FColor LineColor = bHit ? FColor::Red : FColor::Green; // 충돌 여부에 따라 선 색상 설정
	DrawDebugLine(World, Start, End, LineColor, false, 1.0f, 0, 1.0f); // 선 그리기

	return bHit;
}

bool AHitScanGunBaseActor::LineTraceToTarget(AActor* InTarget, TArray<AEnemyBase*>& OutHitTargets)
{
	FVector Direction = InTarget->GetActorLocation() - MuzzleLocation->GetComponentLocation(); // 총구에서 적 캐릭터까지의 방향 벡터
	Direction.Z = 0.0f;			// Z축 방향 제거(2D 평면에서의 거리 계산을 위해)
	Direction.Normalize();		// 방향 벡터 정규화
	float Modifier = OwnerTower->GetBuffModifierValue(ETowerBuffModifier::Range);
	FVector TargetLocation = MuzzleLocation->GetComponentLocation() + CurrentGunData->Range * Modifier * Direction; // 총구에서 적 캐릭터까지의 거리만큼 나간 위치
	return LineTraceToTarget(TargetLocation, OutHitTargets);
}

void AHitScanGunBaseActor::HitProcess()
{
	int32 Count = FMath::Min(CurrentGunData->TargetCount, TargetEnemies.Num()); // 공격할 적의 수
	for (int32 i = 0; i < Count; i++)
	{
		TArray<AEnemyBase*> HitEnemies; // 맞은 적 캐릭터 배열
		bool bHit = LineTraceToTarget(TargetEnemies[i], HitEnemies);
		if (bHit)
		{
			for (auto HitEnemy : HitEnemies)
			{
				// 맞은 적 캐릭터에 대한 처리
				
				// 데미지 처리
				float FinalDamage = CurrentGunData->ShotData->Damage * OwnerTower->GetBuffModifierValue(ETowerBuffModifier::Damage);
				UGameplayStatics::ApplyDamage(
					HitEnemy,		// 적 캐릭터
					FinalDamage,	// 데미지
					nullptr,		// 행위자
					nullptr,		// 공격자
					CurrentGunData->ShotData->AttributeType // 총알의 속성 타입
				);

				// 디버프 처리
				float DebuffModifier = GetModifierForDebuff(CurrentGunData->ShotData->DebuffType);
				HitEnemy->GetDebuffComponent()->AddDebuff(CurrentGunData->ShotData->DebuffType, DebuffModifier); // 디버프 추가
			}			
		}
	}
}
