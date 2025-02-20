// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_Chase.h"
#include "PlayerWingUsePool.h"

APoolingEnemy_Chase::APoolingEnemy_Chase()
{
	PrimaryActorTick.bCanEverTick = true;	
}

void APoolingEnemy_Chase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPawn)
	{
		// 페이즈1: 플레이어가 살아있을 때, 페이즈1 시간이 남았을 때
		if (!bIsPhase1)
		{
			bIsPhase1 = true;	// DoOnce용도
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Phase1 Start : %.1f"), GetWorld()->GetTimeSeconds()));
			ProjectileMovement->bForceSubStepping = false;	// 서브 스텝 강제 해제(천천히 움직일 때는 필요없음)
			DamagePerSecond = Phase1Damage;	// 페이즈1 데미지 설정
		}
		BehaviorPhase1(DeltaTime);	// 페이즈1 행동처리
	}
	else
	{
		// 페이즈2: 플레이어가 죽었을 때, 페이즈1 시간이 다되었을 때 진입
		if (!bIsPhase2)
		{
			bIsPhase2 = true;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Phase2 Start : %.1f"), GetWorld()->GetTimeSeconds()));
			ProjectileMovement->bForceSubStepping = true;	// 서브 스텝 강제(더 상세한 물리 계산)
			DamagePerSecond = Phase2Damage;	// 페이즈2 데미지 설정
		}
		BehaviorPhase2(DeltaTime);	// 페이즈2 행동처리
	}
}

void APoolingEnemy_Chase::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	APlayerWingUsePool* Wing = Cast<APlayerWingUsePool>(PlayerController->GetPawn());
	if (Wing)
	{
		Wing->OnDie.AddDynamic(this, &APoolingEnemy_Chase::TargetLost);	// 플레이어 사망시 TargetPawn을 nullptr로 설정
	}

	ProjectileMovement->MaxSpeed = MaxSpeed;	// 최대 속도 설정
}

void APoolingEnemy_Chase::OnActivate()
{
	Super::OnActivate();

	SetActorTickEnabled(true);					// 틱 활성화

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	TargetPawn = PlayerController->GetPawn();	// TargetPawn 플레이어 폰으로 설정

	FTimerManager& TimerManager = World->GetTimerManager();
	FTimerHandle TempHandle;
	TimerManager.SetTimer(
		TempHandle,
		[this]() { TargetPawn = nullptr; },
		Phase1Duration, false);			// 페이즈1 지속 시간이 끝나면 페이즈2로 진입(TargetPawn을 nullptr로 설정)
}

void APoolingEnemy_Chase::OnDeactivate()
{
	Super::OnDeactivate();

	SetActorTickEnabled(false);	// 틱 비활성화
	TargetPawn = nullptr;		// TargetPawn을 nullptr로 설정

	bIsPhase1 = false;			// 페이즈1 진입용 변수 초기화
	bIsPhase2 = false;			// 페이즈2 진입용 변수 초기화
}

void APoolingEnemy_Chase::BehaviorPhase1(float DeltaTime)
{
	FRotator Current = GetActorRotation();	// 현재 회전값
	FVector Direction = TargetPawn->GetActorLocation() - GetActorLocation();
	Direction.Z = 0;
	FRotator Target = FRotationMatrix::MakeFromX(Direction).Rotator();	// 플레이어를 바라보는 회전값

	FRotator NewRotator = FMath::RInterpConstantTo(Current, Target, DeltaTime, RotateSpeed);	// 회전 보간
	SetActorRotation(NewRotator);			// 보간 결과 적용

	ProjectileMovement->Velocity = MoveSpeed * GetActorForwardVector();	// 회전 된 앞쪽으로 이동
}

void APoolingEnemy_Chase::BehaviorPhase2(float DeltaTime)
{
	// 앞쪽으로 가속
	ProjectileMovement->Velocity += Acceleration * DeltaTime * GetActorForwardVector();

	// 벨로시티의 크기 출력
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Velocity : %.1f"), ProjectileMovement->Velocity.Size()));
}
