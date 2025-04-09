// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanGunBaseActor.h"
#include "TowerDefence/Enemy/EnemyBase.h"

void AHitScanGunBaseActor::Shoot()
{
	Super::Shoot();

	// Line : 선. 시작점과 끝점으로 정의(위치 2개)
	// Ray : 선. 시작점과 방향으로 정의(위치 + 벡터)

	FVector Start = MuzzleLocation->GetComponentLocation();	// 총구 위치
	FVector End = Start + MuzzleLocation->GetForwardVector() * CurrentGunData->Range; // 총구에서 앞쪽으로 사정거리만큼 나간 위치

	UWorld* World = GetWorld();
	FHitResult HitResult; // 충돌 결과를 저장할 변수
	FCollisionQueryParams CollisionParams; // 충돌 쿼리 파라미터
	CollisionParams.AddIgnoredActor(this); // 쿼리에서 무시할 액터 추가

	bool bHit = World->LineTraceSingleByObjectType(
		HitResult,			// 충돌 결과
		Start,				// 시작 위치
		End,				// 끝 위치
		FCollisionObjectQueryParams(ECC_GameTraceChannel1), // 충돌 오브젝트 채널
		CollisionParams
	);

	FColor LineColor = bHit ? FColor::Red : FColor::Green; // 충돌 여부에 따라 선 색상 설정
	DrawDebugLine(World, Start, End, LineColor, false, 1.0f, 0, 1.0f); // 선 그리기

	if (bHit)
	{
		AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitResult.GetActor()); // 충돌한 액터가 적 캐릭터인지 확인
		if (HitEnemy)
		{
			FString TimeString = FDateTime::FromUnixTimestamp(World->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			UE_LOG(LogTemp, Warning, TEXT("[%s] [%s] Hit!"), *TimeString, *HitEnemy->GetActorLabel());
		}
	}
}
